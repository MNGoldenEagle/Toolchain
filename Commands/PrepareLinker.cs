using ManyConsole;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using Toolchain;

namespace Commands
{
    public class PrepareLinker : ConsoleCommand
    {
        public PrepareLinker()
        {
            IsCommand("preparelinker", "Prepares for linking using the current version of the memory table");

            HasLongDescription("Recreates the linker scripts based on the current version of the Memory Table CSV file.");
        }

        public override int Run(string[] remainingArguments)
        {
            var memoryTableFile = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Templates", "Memory Table.csv");

            var memoryTables = GetMemoryTables(File.ReadLines(memoryTableFile));

            foreach (var target in memoryTables.Keys)
            {
                var table = memoryTables[target];
                table.Sort();

                var linkerScript = GenerateLinkerScript(table);
                var linkerScriptFile = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Templates", target.DisplayName);

                File.WriteAllLines(linkerScriptFile, linkerScript, Encoding.UTF8);
            }

            return 0;
        }

        private List<string> GenerateLinkerScript(MemoryTable table)
        {
            var lines = new List<string>();
            WriteMemorySection(lines);
            WriteSections(lines);
            WriteSymbols(table, lines);
            FinishSections(lines);
            return lines;
        }

        private void WriteMemorySection(List<string> lines)
        {
            lines.Add("MEMORY {");
            lines.Add("\tcode    : ORIGIN = 0x80000000, LENGTH = 0x00300000");
            lines.Add("\toverlay : ORIGIN = 0x80800000, LENGTH = 0x00100000");
            lines.Add("}");
            lines.Add(string.Empty);
        }

        private void WriteSections(List<string> lines)
        {
            lines.Add("SECTIONS {");
            lines.Add("\t.text : {");
            lines.Add("\t\t*(\".text\")");
            lines.Add("\t} > overlay");
            lines.Add("\t.data : {");
            lines.Add("\t\t*(\".data\")");
            lines.Add("\t} > overlay");
            lines.Add("\t.rodata : {");
            lines.Add("\t\t*(\".rodata\")");
            lines.Add("\t} > overlay");
            lines.Add("\t.bss : {");
            lines.Add("\t\t*(\".bss\")");
            lines.Add("\t} > overlay");
            lines.Add("\tz64 : {");
        }

        private void WriteSymbols(MemoryTable table, List<string> lines)
        {
            var categorizedTable = table.GroupBy(entry => entry.Category).ToDictionary(entry => entry.Key, entry => entry.ToList());
            bool first = true;

            foreach (var categorizedEntries in categorizedTable)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    lines.Add("");
                }
                lines.Add(string.Format("\t\t/* {0} */", categorizedEntries.Key));
                foreach (var entry in categorizedEntries.Value)
                {
                    lines.Add(string.Format("\t\t{0} = . + {1:X};", entry.SymbolName, entry.Offset));
                }
            }
        }

        private void FinishSections(List<string> lines)
        {
            lines.Add("\t} > code");
            lines.Add("}");
        }

        private IReadOnlyDictionary<Target, MemoryTable> GetMemoryTables(IEnumerable<string> rawEntries)
        {
            var result = new SortedDictionary<Target, MemoryTable>();

            var enumerator = rawEntries.GetEnumerator();
            enumerator.MoveNext(); // Skip header line

            if (enumerator.Current == null)
            {
                return result;
            }

            while (enumerator.MoveNext())
            {
                var tokens = enumerator.Current.Split(',');
                
                if (tokens.Length != 9)
                {
                    throw new FormatException("The memory table file must contain nine values per line. See README for details.");
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

        private void ParseToken(string token, string symbol, string category, Target target, SortedDictionary<Target, MemoryTable> dict)
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

        private MemoryTable GetMemoryTable(SortedDictionary<Target, MemoryTable> dict, Target target)
        {
            if (!dict.ContainsKey(target))
            {
                dict[target] = new MemoryTable();
            }

            return dict[target];
        }

        private class MemoryTable : List<TableEntry>
        {
        }

        private struct TableEntry : IComparable<TableEntry>
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
}
