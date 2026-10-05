#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Copyright (c) 2026 marth
"""mit_idtable.py -- build, check and dump MIT id table files (docs/MIT-ID-TABLE-FORMAT.md, format 1.0).

  build  CSV(s) + the exact game executable -> mit-idtable-v1-<ver>.bin
  check  a .bin against the format rules, and against an executable if one is given
  dump   a .bin as a header summary plus id,rva CSV lines

Only the Python standard library is needed, plus `pefile` for reading the executable's ProductVersion
(`pip install pefile`).

The CSV needs one column with the decimal id and one with the RVA (hex with 0x, or decimal). Any other
columns can be used to filter rows with --where. Every row that survives the filters MUST have a numeric
id and an RVA. A row that cannot be used is an error, not a skip, so a table never silently loses ids.
"""
import argparse
import csv
import os
import struct
import sys

MAGIC = b'MITIDTAB'
FORMAT_MAJOR = 1          # in the file name; a reader refuses any other major
FORMAT_MINOR = 0          # a reader of major 1 reads every minor
HEADER_SIZE = 64          # what this tool writes; readers accept >= 64 and skip the rest
RECORD_SIZE = 16          # what this tool writes; readers accept >= 16 and skip the rest
MAX_RECORD_SIZE = 4096
TRAILER_SIZE = 8
HEADER_FMT = '<8sHHIHHHHIIIIII16s'
assert struct.calcsize(HEADER_FMT) == HEADER_SIZE


def file_name(version):
    return 'mit-idtable-v%d-%s.bin' % (FORMAT_MAJOR, '-'.join(str(x) for x in version))

FNV_OFFSET = 0xCBF29CE484222325
FNV_PRIME = 0x100000001B3
MASK64 = (1 << 64) - 1


def fnv1a64(data):
    h = FNV_OFFSET
    for b in data:
        h ^= b
        h = (h * FNV_PRIME) & MASK64
    return h


class ExeInfo:
    """What the reader compares against: ProductVersion, PE TimeDateStamp, SizeOfImage, file name, sections."""

    def __init__(self, path):
        self.path = path
        self.name = os.path.basename(path)
        d = open(path, 'rb').read()
        pe = struct.unpack_from('<I', d, 0x3C)[0]
        if d[pe:pe + 4] != b'PE\0\0':
            raise SystemExit('%s: not a PE file' % path)
        self.time_date_stamp = struct.unpack_from('<I', d, pe + 8)[0]
        nsec = struct.unpack_from('<H', d, pe + 6)[0]
        opt_size = struct.unpack_from('<H', d, pe + 20)[0]
        opt = pe + 24
        if struct.unpack_from('<H', d, opt)[0] != 0x20B:
            raise SystemExit('%s: not a PE32+ (64-bit) image' % path)
        self.size_of_image = struct.unpack_from('<I', d, opt + 56)[0]
        sec = opt + opt_size
        self.sections = []
        for i in range(nsec):
            n = d[sec + i * 40:sec + i * 40 + 8].rstrip(b'\0').decode(errors='replace')
            vs, va = struct.unpack_from('<II', d, sec + i * 40 + 8)
            rs = struct.unpack_from('<I', d, sec + i * 40 + 16)[0]
            self.sections.append((n, va, max(vs, rs)))
        self.version = self._product_version(path)

    @staticmethod
    def _product_version(path):
        # The fork reads the version from StringFileInfo\040904B0\ProductVersion (REL::get_file_version),
        # not from VS_FIXEDFILEINFO (1.5.97's fixed info says 1.0.0.0). Read the same string here.
        try:
            import pefile
        except ImportError:
            raise SystemExit('pefile is needed to read the ProductVersion: pip install pefile')
        pe = pefile.PE(path, fast_load=True)
        pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_RESOURCE']])
        for fi in getattr(pe, 'FileInfo', [[]])[0]:
            for st in getattr(fi, 'StringTable', []):
                if st.LangID.lower() == b'040904b0' and b'ProductVersion' in st.entries:
                    parts = st.entries[b'ProductVersion'].decode().strip().split('.')
                    v = [int(p) for p in parts[:4]] + [0] * (4 - len(parts[:4]))
                    return tuple(v)
        raise SystemExit('%s: no StringFileInfo\\040904B0\\ProductVersion' % path)

    def section_of(self, rva):
        for n, va, size in self.sections:
            if va <= rva < va + size:
                return n
        return None


def parse_int(text, what):
    t = text.strip()
    try:
        return int(t, 16) if t.lower().startswith('0x') else int(t, 10)
    except ValueError:
        raise ValueError('%s %r is not a number' % (what, text))


def parse_where(items):
    out = []
    for item in items or []:
        if '=' not in item:
            raise SystemExit('--where %r: expected column=value[,value...]' % item)
        col, vals = item.split('=', 1)
        out.append((col, set(v for v in vals.split(','))))
    return out


def read_rows(paths, id_col, rva_col, where, absent_where=()):
    """-> {id: (rva, where)}. rva 0 = an absent record (the row matches every --absent-where)."""
    rows = {}
    kept = dropped = absent = 0
    for path in paths:
        with open(path, newline='') as f:
            reader = csv.DictReader(f)
            for col in [id_col, rva_col] + [c for c, _ in where] + [c for c, _ in absent_where]:
                if col not in (reader.fieldnames or []):
                    raise SystemExit('%s: no column %r (columns: %s)' % (path, col, ', '.join(reader.fieldnames or [])))
            for lineno, row in enumerate(reader, start=2):
                where_txt = '%s:%d' % (path, lineno)
                is_absent = bool(absent_where) and all(row[c] in vals for c, vals in absent_where)
                if not is_absent and any(row[c] not in vals for c, vals in where):
                    dropped += 1
                    continue
                try:
                    i = parse_int(row[id_col], 'id')
                    if is_absent:
                        r = 0
                    else:
                        if not row[rva_col].strip():
                            raise ValueError('id %d has no rva in column %r' % (i, rva_col))
                        r = parse_int(row[rva_col], 'rva')
                        if r == 0:
                            raise ValueError('id %d has rva 0 (only --absent-where rows may be absent)' % i)
                except ValueError as e:
                    raise SystemExit('%s: %s (filter it out with --where, or fix the row)' % (where_txt, e))
                if i < 0 or i >= 1 << 64:
                    raise SystemExit('%s: id %d out of range' % (where_txt, i))
                if i in rows and rows[i][0] != r:
                    raise SystemExit('%s: id %d maps to 0x%X here and 0x%X at %s (0 = absent)' % (
                        where_txt, i, r, rows[i][0], rows[i][1]))
                if i not in rows:
                    rows[i] = (r, where_txt)
                    if is_absent:
                        absent += 1
                    else:
                        kept += 1
    return rows, kept, dropped, absent


def pack(version, tds, soi, module, revision, records):
    name = module.encode('ascii')
    if len(name) > 15:
        raise SystemExit('module name %r longer than 15 bytes' % module)
    if not 1 <= revision < 1 << 32:
        raise SystemExit('table revision must be 1 or more')
    hdr = struct.pack(HEADER_FMT, MAGIC, FORMAT_MAJOR, FORMAT_MINOR, HEADER_SIZE, *version, tds, soi, len(records),
                      RECORD_SIZE, 0, revision, name.ljust(16, b'\0'))
    body = b''.join(struct.pack('<QQ', i, r) for i, r in records)
    data = hdr + body
    return data + struct.pack('<Q', fnv1a64(data))


def unpack(data, label):
    """Apply the format rules in the reader's order (docs/MIT-ID-TABLE-FORMAT.md). Returns (header dict, records)."""
    def fail(msg):
        raise SystemExit('%s: %s' % (label, msg))
    if len(data) < HEADER_SIZE + TRAILER_SIZE:
        fail('file is %d bytes, smaller than a header and a trailer' % len(data))
    (magic, major, minor, hsize, v0, v1, v2, v3, tds, soi, count, rsize, flags, revision,
     module) = struct.unpack_from(HEADER_FMT, data, 0)
    if magic != MAGIC:
        fail('magic is %r, not %r' % (magic, MAGIC))
    if major != FORMAT_MAJOR:
        fail('format %d.%d, this tool reads %d.x' % (major, minor, FORMAT_MAJOR))
    if hsize < HEADER_SIZE or rsize < RECORD_SIZE or hsize > len(data) or rsize > MAX_RECORD_SIZE:
        fail('headerSize %d / recordSize %d not valid (at least %d / %d)' % (hsize, rsize, HEADER_SIZE, RECORD_SIZE))
    if flags != 0:
        fail('flags 0x%X are not known to format %d' % (flags, FORMAT_MAJOR))
    want = hsize + rsize * count + TRAILER_SIZE
    if len(data) != want:
        fail('file is %d bytes, recordCount %d needs exactly %d' % (len(data), count, want))
    stored = struct.unpack_from('<Q', data, len(data) - TRAILER_SIZE)[0]
    actual = fnv1a64(data[:-TRAILER_SIZE])
    if stored != actual:
        fail('checksum 0x%016X, computed 0x%016X' % (stored, actual))
    if revision == 0:
        fail('table revision is 0')
    records = [struct.unpack_from('<QQ', data, hsize + rsize * k) for k in range(count)]
    prev = None
    for i, r in records:
        if prev is not None and i <= prev:
            fail('records not strictly ascending at id %d (after %d)' % (i, prev))
        if r >= soi:
            fail('id %d has rva 0x%X, outside SizeOfImage 0x%X' % (i, r, soi))
        prev = i
    hdr = dict(format=(major, minor), version=(v0, v1, v2, v3), time_date_stamp=tds, size_of_image=soi,
               count=count, revision=revision, module=module.rstrip(b'\0').decode('ascii', 'replace'),
               checksum=stored)
    return hdr, records


def check_against_exe(hdr, records, exe, label, module=None):
    problems = []
    if hdr['version'] != exe.version:
        problems.append('gameVersion %s, executable is %s' % (hdr['version'], exe.version))
    if hdr['module'].lower() != (module or exe.name).lower():
        problems.append('moduleName %r, expected %r' % (hdr['module'], module or exe.name))
    if hdr['time_date_stamp'] != exe.time_date_stamp:
        problems.append('peTimeDateStamp 0x%X, executable 0x%X' % (hdr['time_date_stamp'], exe.time_date_stamp))
    if hdr['size_of_image'] != exe.size_of_image:
        problems.append('peSizeOfImage 0x%X, executable 0x%X' % (hdr['size_of_image'], exe.size_of_image))
    for i, r in records:
        if r != 0 and exe.section_of(r) is None:
            problems.append('id %d rva 0x%X is in no section of %s' % (i, r, exe.name))
    if problems:
        raise SystemExit('%s:\n  ' % label + '\n  '.join(problems))


def ver_name(v):
    return '-'.join(str(x) for x in v)


def parse_corrections(a):
    """--correct ID[,ID] + --correct-evidence CSV (id, previous_rva, new_rva, evidence): the audited way to change a
    published RVA that was proven wrong. Every listed id needs exactly one evidence row with non-empty evidence."""
    ids = set()
    for item in a.correct or []:
        for x in item.split(','):
            if x.strip():
                ids.add(parse_int(x, 'id'))
    if not ids:
        if a.correct_evidence:
            raise SystemExit('--correct-evidence without --correct')
        return {}
    if not a.correct_evidence:
        raise SystemExit('--correct needs --correct-evidence (a CSV: id,previous_rva,new_rva,evidence)')
    rows = {}
    with open(a.correct_evidence, newline='') as f:
        for lineno, row in enumerate(csv.DictReader(f), start=2):
            try:
                i = parse_int(row['id'], 'id')
                old = parse_int(row['previous_rva'], 'previous_rva')
                new = parse_int(row['new_rva'], 'new_rva') if row['new_rva'].strip().lower() != 'absent' else 0
            except (KeyError, ValueError) as e:
                raise SystemExit('%s:%d: %s' % (a.correct_evidence, lineno, e))
            if not (row.get('evidence') or '').strip():
                raise SystemExit('%s:%d: id %d has no evidence' % (a.correct_evidence, lineno, i))
            if i in rows:
                raise SystemExit('%s:%d: id %d has two evidence rows' % (a.correct_evidence, lineno, i))
            rows[i] = (old, new, row['evidence'].strip())
    missing = sorted(ids - set(rows))
    extra = sorted(set(rows) - ids)
    if missing or extra:
        raise SystemExit('--correct and --correct-evidence disagree: no evidence for %s, evidence but not --correct for %s'
                         % (missing, extra))
    return rows


def module_name(a, exe):
    if a.module.lower() != exe.name.lower():
        print('warning: the executable file is named %r; the table names the module %r (the name the game runs '
              'under). Pass --module if that is wrong.' % (exe.name, a.module), file=sys.stderr)
    return a.module


def cmd_build(a):
    exe = ExeInfo(a.exe)
    module = module_name(a, exe)
    where = parse_where(a.where)
    rows, kept, dropped, absent = read_rows(a.csv, a.id_col, a.rva_col, where, parse_where(a.absent_where))
    if not rows:
        raise SystemExit('no rows left after the filters')
    for i, (r, src) in rows.items():
        if r != 0 and (r >= exe.size_of_image or exe.section_of(r) is None):
            raise SystemExit('%s: id %d rva 0x%X is not inside %s' % (src, i, r, exe.name))
    records = sorted((i, r) for i, (r, _) in rows.items())
    corrections = parse_corrections(a)
    if a.previous:
        # Distribution rule: a published table is a strict superset of the one before it.
        phdr, prec = unpack(open(a.previous, 'rb').read(), a.previous)
        # The previous table must be for the same executable, or the comparison means nothing.
        if (phdr['version'] != exe.version or phdr['time_date_stamp'] != exe.time_date_stamp or
                phdr['size_of_image'] != exe.size_of_image):
            raise SystemExit('%s is for game %s, TimeDateStamp 0x%08X, SizeOfImage 0x%X; --exe is %s, 0x%08X, 0x%X' % (
                a.previous, '.'.join(map(str, phdr['version'])), phdr['time_date_stamp'], phdr['size_of_image'],
                '.'.join(map(str, exe.version)), exe.time_date_stamp, exe.size_of_image))
        have = dict(records)
        prev = dict(prec)
        lost = [i for i, _ in prec if i not in have]
        # a mapped id stays mapped at the same RVA; an absent id may only stay absent or become mapped
        moved = [i for i, r in prec if i in have and have[i] != r and r != 0 and i not in corrections]
        if lost or moved:
            raise SystemExit('not a superset of %s: %d ids dropped, %d ids changed RVA (first: %s). A proven-wrong RVA '
                             'is fixed with --correct and --correct-evidence.' % (
                                 a.previous, len(lost), len(moved), (lost + moved)[:5]))
        for i, (old, new, evidence) in sorted(corrections.items()):
            if prev.get(i) != old:
                raise SystemExit('--correct %d: the previous table has 0x%X, the evidence row says it had 0x%X' % (
                    i, prev.get(i, 0), old))
            if have.get(i) != new:
                raise SystemExit('--correct %d: this build gives 0x%X, the evidence row says 0x%X' % (i, have.get(i, 0), new))
            print('CORRECTION id %d: 0x%X -> 0x%X (%s)' % (i, old, new, evidence))
        if a.revision <= phdr['revision']:
            raise SystemExit('--revision %d must be above the previous table\'s %d' % (a.revision, phdr['revision']))
    elif corrections:
        raise SystemExit('--correct needs --previous (a correction is a change against a published table)')
    data = pack(exe.version, exe.time_date_stamp, exe.size_of_image, module, a.revision, records)
    out = a.out or file_name(exe.version)
    if os.path.isdir(out):
        out = os.path.join(out, file_name(exe.version))
    # Prove the bytes we are about to write read back under the same rules the fork applies.
    hdr, back = unpack(data, out)
    check_against_exe(hdr, back, exe, out, module)
    assert back == records
    with open(out, 'wb') as f:
        f.write(data)
    print('wrote %s: format %d.%d, revision %d, %d records (%d mapped, %d absent; %d CSV rows filtered out), game %s, '
          'module %s, TimeDateStamp 0x%08X, SizeOfImage 0x%X, checksum 0x%016X' % (
              out, FORMAT_MAJOR, FORMAT_MINOR, a.revision, len(records), kept, absent, dropped,
              '.'.join(map(str, exe.version)), module, exe.time_date_stamp, exe.size_of_image, hdr['checksum']))


def cmd_check(a):
    data = open(a.bin, 'rb').read()
    hdr, records = unpack(data, a.bin)
    if a.exe:
        check_against_exe(hdr, records, ExeInfo(a.exe), a.bin, a.module)
    if a.csv:
        rows, _, _, _ = read_rows(a.csv, a.id_col, a.rva_col, parse_where(a.where), parse_where(a.absent_where))
        want = sorted((i, r) for i, (r, _) in rows.items())
        if want != records:
            have = dict(records)
            missing = [i for i, _ in want if i not in have]
            extra = [i for i, _ in records if i not in dict(want)]
            differ = [i for i, r in want if i in have and have[i] != r]
            raise SystemExit('%s does not match the CSV: %d missing, %d extra, %d differ (first: %s)' % (
                a.bin, len(missing), len(extra), len(differ), (missing + extra + differ)[:5]))
    print('ok %s: format %d.%d, revision %d, game %s, module %s, TimeDateStamp 0x%08X, SizeOfImage 0x%X, %d records, checksum 0x%016X%s' % (
        a.bin, hdr['format'][0], hdr['format'][1], hdr['revision'], '.'.join(map(str, hdr['version'])), hdr['module'], hdr['time_date_stamp'],
        hdr['size_of_image'], hdr['count'], hdr['checksum'], ', matches the executable' if a.exe else ''))


def cmd_dump(a):
    hdr, records = unpack(open(a.bin, 'rb').read(), a.bin)
    print('# format %d.%d, revision %d, game %s, module %s, TimeDateStamp 0x%08X, SizeOfImage 0x%X, %d records, checksum 0x%016X' % (
        hdr['format'][0], hdr['format'][1], hdr['revision'], '.'.join(map(str, hdr['version'])), hdr['module'], hdr['time_date_stamp'],
        hdr['size_of_image'], hdr['count'], hdr['checksum']))
    print('id,rva')
    for i, r in records:
        print('%d,%s' % (i, '0x%X' % r if r else 'absent'))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)

    def csv_args(p, required):
        p.add_argument('--csv', action='append', required=required, help='input CSV (repeatable; ids must agree)')
        p.add_argument('--id-col', default='id', help='column with the decimal id (default: id)')
        p.add_argument('--rva-col', default='rva', help='column with the RVA (default: rva)')
        p.add_argument('--where', action='append', metavar='COL=V1,V2',
                       help='keep only rows whose COL is one of the values (repeatable, all must hold)')
        p.add_argument('--absent-where', action='append', metavar='COL=V1,V2',
                       help='rows matching every one of these become ABSENT records (rva 0: known not to exist in '
                            'this executable); they are not subject to --where')

    b = sub.add_parser('build', help='CSV(s) + executable -> .bin')
    b.add_argument('--exe', required=True, help='the exact game executable the RVAs were taken from')
    b.add_argument('--out', help='output file or directory (default: ./mit-idtable-v1-<version>.bin)')
    b.add_argument('--revision', type=int, required=True,
                   help='table revision, 1 or more, higher than every table published before for this game version')
    b.add_argument('--previous', help='the last published table: the build refuses to drop or move any of its ids')
    b.add_argument('--module', default='SkyrimSE.exe', help='module name the game runs under (default: SkyrimSE.exe)')
    b.add_argument('--correct', action='append', metavar='ID[,ID]',
                   help='ids whose published RVA was proven wrong and may change against --previous (audited)')
    b.add_argument('--correct-evidence', metavar='CSV',
                   help='evidence for --correct: a CSV with id,previous_rva,new_rva,evidence (one row per id)')
    csv_args(b, True)
    b.set_defaults(func=cmd_build)

    c = sub.add_parser('check', help='validate a .bin (and optionally against an executable and a CSV)')
    c.add_argument('bin')
    c.add_argument('--exe')
    c.add_argument('--module', default='SkyrimSE.exe', help='expected module name (default: SkyrimSE.exe)')
    csv_args(c, False)
    c.set_defaults(func=cmd_check)

    d = sub.add_parser('dump', help='print a .bin as CSV')
    d.add_argument('bin')
    d.set_defaults(func=cmd_dump)

    a = ap.parse_args(argv)
    a.func(a)


if __name__ == '__main__':
    main()
