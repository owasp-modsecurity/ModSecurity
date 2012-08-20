using System;

namespace configure
{
    public class Program
    {
        static void Main(string[] args)
        {
            ModSecurityConfigurator configurator = new ModSecurityConfigurator();

            if (args.Length > 0) 
            {
                if (args[0].Equals("uninstall", StringComparison.InvariantCultureIgnoreCase))
                    configurator.Uninstall();
                else
                {
                    configurator.Install(args[0]);
                }
            }
            else
            {
                Console.WriteLine("configure.exe <install dir> | uninstall");
            }
        }
    }
}
