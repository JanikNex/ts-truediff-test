import difflib as dl
import sys


def tmp():
    a = ""
    b = ""
    raw_edit = []

    old_point_column = len(a[raw_edit[1]:raw_edit[2]].rsplit("\n", 1)[-1])
    new_point_column = len(b[raw_edit[3]:raw_edit[4]].rsplit("\n", 1)[-1])


def find_diff(from_name, to_name, verbose):
    with open(from_name, "r") as a_file, open(to_name, "r") as b_file:
        a = a_file.read()
        b = b_file.read()

        s = dl.SequenceMatcher(None, a, b)
        raw_edits = []
        edits = []
        if verbose:
            for tag, i1, i2, j1, j2 in s.get_opcodes():
                print('{:7}   a[{}:{}] --> b[{}:{}] {!r:>8} --> {!r}'.format(
                    tag, i1, i2, j1, j2, a[i1:i2], b[j1:j2]))
                raw_edits += [[tag, i1, i2, j1, j2]]
            print('Found {} raw edits!'.format(len(raw_edits)))
        else:
            for tag, i1, i2, j1, j2 in s.get_opcodes():
                raw_edits += [[tag, i1, i2, j1, j2]]
        start_pos = 0
        start_row = 0
        start_column = 0
        if len(raw_edits) > 0:
            if verbose:
                print('Converting...')
            start_pos = raw_edits[0][1]
        for raw_edit in raw_edits:
            if raw_edit[0] == 'equal':
                start_pos += raw_edit[2] - raw_edit[1]
                new_rows = a[raw_edit[1]:raw_edit[2]].count("\n")
                start_row += new_rows
                if new_rows == 0:
                    start_column += len(b[raw_edit[3]:raw_edit[4]].rsplit("\n", 1)[-1])
                else:
                    start_column = len(b[raw_edit[3]:raw_edit[4]].rsplit("\n", 1)[-1])
            if raw_edit[0] == 'insert':
                # Bytes
                start = start_pos
                old_end = start
                edit_len = raw_edit[4] - raw_edit[3]
                new_end = old_end + edit_len
                # Points
                start_point_row = start_row
                insert_rows = b[raw_edit[3]:raw_edit[4]].count("\n")
                end_row = start_point_row + insert_rows
                start_point_column = start_column
                new_end_column = len(b[raw_edit[3]:raw_edit[4]].rsplit("\n", 1)[-1])
                if insert_rows == 0:
                    new_end_column += start_column
                start_column = new_end_column

                edits += [[start,
                           start_point_row,
                           start_point_column,
                           old_end,
                           start_point_row,
                           start_point_column,
                           new_end,
                           end_row,
                           new_end_column]]
                if verbose:
                    print('[INSERT] Start: {} ({}|{}), Old end: {} ({}|{}), New end: {} ({}|{})'.format(start,
                                                                                                        start_point_row,
                                                                                                        start_point_column,
                                                                                                        old_end,
                                                                                                        start_point_row,
                                                                                                        start_point_column,
                                                                                                        new_end,
                                                                                                        end_row,
                                                                                                        new_end_column))
                start_pos += new_end - old_end
                start_row += insert_rows
                continue
            if raw_edit[0] == 'replace':
                # Bytes
                start = start_pos
                old_len = raw_edit[2] - raw_edit[1]
                new_len = raw_edit[4] - raw_edit[3]
                old_end = start + old_len
                new_end = start + new_len
                # Points
                start_point_row = start_row
                old_rows = a[raw_edit[1]:raw_edit[2]].count("\n")
                new_rows = b[raw_edit[3]:raw_edit[4]].count("\n")
                old_end_row = start_point_row + old_rows
                new_end_row = start_point_row + new_rows
                start_point_column = start_column
                old_end_column = len(a[raw_edit[1]:raw_edit[2]].rsplit("\n", 1)[-1])
                if old_rows == 0:
                    old_end_column += start_column
                new_end_column = len(b[raw_edit[3]:raw_edit[4]].rsplit("\n", 1)[-1])
                if new_rows == 0:
                    new_end_column += start_column
                start_column = new_end_column

                edits += [[start,
                           start_point_row,
                           start_point_column,
                           old_end,
                           old_end_row,
                           old_end_column,
                           new_end,
                           new_end_row,
                           new_end_column]]
                if verbose:
                    print('[UPDATE] Start: {} ({}|{}), Old end: {} ({}|{}), New end: {} ({}|{})'.format(start,
                                                                                                        start_point_row,
                                                                                                        start_point_column,
                                                                                                        old_end,
                                                                                                        old_end_row,
                                                                                                        old_end_column,
                                                                                                        new_end,
                                                                                                        new_end_row,
                                                                                                        new_end_column))
                start_pos += new_len
                start_row += new_rows
                continue
            if raw_edit[0] == 'delete':
                # Bytes
                start = start_pos
                edit_len = raw_edit[2] - raw_edit[1]
                old_end = start + edit_len
                new_end = start
                # Points
                start_point_row = start_row
                old_rows = a[raw_edit[1]:raw_edit[2]].count("\n")
                old_point_row = start_point_row + old_rows
                start_point_column = start_column
                old_end_column = len(a[raw_edit[1]:raw_edit[2]].rsplit("\n", 1)[-1])
                if old_rows == 0:
                    old_end_column += start_column
                new_end_column = start_column

                edits += [[start,
                           start_point_row,
                           start_point_column,
                           old_end,
                           old_point_row,
                           old_end_column,
                           new_end,
                           start_point_row,
                           new_end_column]]
                if verbose:
                    print('[DELETE] Start: {} ({}|{}), Old end: {} ({}|{}), New end: {} ({}|{})'.format(start,
                                                                                                        start_point_row,
                                                                                                        start_point_column,
                                                                                                        old_end,
                                                                                                        old_point_row,
                                                                                                        old_end_column,
                                                                                                        new_end,
                                                                                                        start_point_row,
                                                                                                        new_end_column))
                continue
    return edits


if __name__ == '__main__':
    if len(sys.argv) == 3:
        find_diff(sys.argv[1], sys.argv[2], False)
    else:
        find_diff("from.txt", "to.txt", True)
