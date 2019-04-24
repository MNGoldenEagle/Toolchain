using ManyConsole;
using System;
using System.Collections.Generic;
using System.IO;
using Toolchain;

namespace Toolchain
{
    public class Pj64Symbols : ConsoleCommand
    {
        public string OutputFile { get; set; }

        public Target Target { get; set; }

        public Pj64Symbols()
        {
            IsCommand("pj64sym", "Writes a Project64 symbols file for all known variables and functions");

            HasLongDescription("Generates a Project64 symbols file based on the current version of the Memory Table CSV file.");

            HasRequiredOption<string>("t|target=", "Specifies the target ROM type (e.g., 1.0U, 1.1U, 1.2E, Debug)", value => Target = Target.FromValue(value.ToUpperInvariant()));

            HasRequiredOption<string>("o|output=", "Specifies the output filename", value => OutputFile = value);
        }

        public override int Run(string[] remainingArguments)
        {
            var memoryTableFile = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Templates", "Memory Table.csv");

            var memoryTables = MemoryTable.GetMemoryTables(File.ReadLines(memoryTableFile));

            var table = memoryTables[Target];
            table.Sort();

            var symbols = GenerateProject64Symbols(table);
            var symoblFile = OutputFile;

            File.WriteAllLines(OutputFile, symbols);

            return 0;
        }

        private List<string> GenerateProject64Symbols(MemoryTable table)
        {
            var lines = new List<string>();

            foreach (TableEntry entry in table)
            {
                var address = 0x80000000 + entry.Offset;
                lines.Add(string.Format("{0:X},code,{1},{2}", address, entry.SymbolName, entry.Category));
            }

            return lines;
        }
    }
}
