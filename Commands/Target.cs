using Headspring;

namespace Toolchain
{
    public class Target : Enumeration<Target, string>
    {
        public static readonly Target NTSC_1_0 = new Target("1.0U", "NTSC 1.0.ld");
        public static readonly Target NTSC_1_0J = new Target("1.0J", "NTSC 1.0.ld");
        public static readonly Target NTSC_1_1 = new Target("1.1U", "NTSC 1.1.ld");
        public static readonly Target NTSC_1_1J = new Target("1.1J", "NTSC 1.1.ld");
        public static readonly Target NTSC_1_2 = new Target("1.2U", "NTSC 1.2.ld");
        public static readonly Target NTSC_1_2J = new Target("1.2J", "NTSC 1.2.ld");
        public static readonly Target NTSC_MQ = new Target("MQU", "NTSC MQ.ld");
        public static readonly Target PAL_1_0 = new Target("1.0E", "PAL 1.0.ld");
        public static readonly Target PAL_1_1 = new Target("1.1E", "PAL 1.1.ld");
        public static readonly Target PAL_MQ_DEBUG = new Target("DEBUG", "Debug.ld");

        private Target(string value, string displayName) : base(value, displayName) { }
    }
}
