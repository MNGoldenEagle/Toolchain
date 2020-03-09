using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace Toolchain
{
    public class MemoryTable : List<TableEntry>
    {
        public static IReadOnlyDictionary<Target, MemoryTable> GetMemoryTables(IEnumerable<string> rawEntries)
        {
            var result = new SortedDictionary<Target, MemoryTable>();

            var enumerator = rawEntries.GetEnumerator();
            var lineNumber = 0;
            enumerator.MoveNext(); // Skip header line
            lineNumber++;

            if (enumerator.Current == null)
            {
                return result;
            }

            while (enumerator.MoveNext())
            {
                lineNumber++;
                var tokens = enumerator.Current.Split(',');

                // Skip blank lines (or lines that are comments) so that we can have some sane visual grouping between different categories.
                if (string.IsNullOrWhiteSpace(enumerator.Current) || enumerator.Current.StartsWith("#"))
                {
                    continue;
                }

                if (tokens.Length != 9)
                {
                    throw new FormatException(String.Format("The memory table file must contain nine values per line (review line {0}). See README for details.", lineNumber));
                }

                var symbol = tokens[0];
                var category = tokens[1];

                ParseToken(tokens[2], symbol, category, Target.NTSC_1_0, result);
                ParseToken(tokens[3], symbol, category, Target.NTSC_1_1, result);
                ParseToken(tokens[4], symbol, category, Target.NTSC_1_2, result);
                ParseToken(tokens[5], symbol, category, Target.PAL_1_0, result);
                ParseToken(tokens[6], symbol, category, Target.PAL_1_1, result);
                ParseToken(tokens[7], symbol, category, Target.NTSC_MQ, result);
                ParseToken(tokens[8], symbol, category, Target.PAL_MQ_DEBUG, result);
            };

            return result;
        }

        private static void ParseToken(string token, string symbol, string category, Target target, SortedDictionary<Target, MemoryTable> dict)
        {
            uint offset = 0;
            uint.TryParse(token, NumberStyles.HexNumber, null, out offset);

            if (offset == 0)
            {
                return;
            }

            var memoryTable = GetMemoryTable(dict, target);

            memoryTable.Add(new TableEntry
            {
                SymbolName = symbol,
                Category = category,
                Offset = offset
            });
        }

        private static MemoryTable GetMemoryTable(SortedDictionary<Target, MemoryTable> dict, Target target)
        {
            if (!dict.ContainsKey(target))
            {
                dict[target] = new MemoryTable();
            }

            return dict[target];
        }
    }

    public struct TableEntry : IComparable<TableEntry>
    {
        public string SymbolName;
        public string Category;
        public uint Offset;

        public int CompareTo(TableEntry other)
        {
            int difference = string.Compare(Category, other.Category);

            if (difference == 0)
            {
                difference = (int)Offset - (int)other.Offset;
            }

            return difference;
        }
    }
}
