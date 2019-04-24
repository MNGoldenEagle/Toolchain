using ManyConsole;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
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

            var memoryTables = MemoryTable.GetMemoryTables(File.ReadLines(memoryTableFile));

            foreach (var target in memoryTables.Keys)
            {
                var table = memoryTables[target];
                table.Sort();

                var linkerScript = GenerateLinkerScript(table);
                var linkerScriptFile = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Templates", target.DisplayName);

                File.WriteAllLines(linkerScriptFile, linkerScript);
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
            lines.Add("\t\t*(\".rodata*\")");
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
                    lines.Add(string.Format("\t\t{0} = . + 0x{1:X};", entry.SymbolName, entry.Offset));
                }
            }
        }

        private void FinishSections(List<string> lines)
        {
            lines.Add("\t} > code");
            lines.Add("}");
        }
    }
}
