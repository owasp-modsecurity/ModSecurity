using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using AppHostAdminLibrary;
using System.Runtime.InteropServices;

namespace configure
{
    public class ModSecurityConfigurator
    {
        public void Install(string installDir)
        {
            if (installDir.EndsWith("\""))
            {
                installDir = installDir.Substring(0, installDir.Length - 1);
            }
            if (installDir.StartsWith("\""))
            {
                installDir = installDir.Substring(1);
            }

            Console.WriteLine("Copying 32-bit binaries...");
            string dstpath = Environment.ExpandEnvironmentVariables("%windir%\\SysWow64");

            if (!Directory.Exists(dstpath))
                dstpath = Environment.ExpandEnvironmentVariables("%windir%\\system32");

            CopyBinaries(Path.Combine(installDir, "x86"), Path.Combine(dstpath, "inetsrv"));

            dstpath = Environment.ExpandEnvironmentVariables("%windir%\\SysNative");

            if (Directory.Exists(dstpath))
            {
                Console.WriteLine("Copying 64-bit binaries...");

                CopyBinaries(Path.Combine(installDir, "amd64"), Path.Combine(dstpath, "inetsrv"));
            }

            string text = Path.Combine(installDir, "ModSecurity.xml");
            string text2 = null;
            IntPtr zero = IntPtr.Zero;
            try
            {
                bool flag = false;
                if (Wow64Redirection.IsWow64Process(new IntPtr(-1), out flag) && flag)
                {
                    Wow64Redirection.Wow64DisableWow64FsRedirection(out zero);
                }
                if (!File.Exists(text))
                {
                    throw new FileNotFoundException("The specified schema file does not exist", text);
                }
                Console.WriteLine("Installing schema file: " + text);
                text2 = Path.Combine(Environment.ExpandEnvironmentVariables("%windir%\\system32\\inetsrv\\config\\schema\\"), Path.GetFileName(text));
                if (!text2.Equals(text, StringComparison.InvariantCultureIgnoreCase))
                {
                    File.Copy(text, text2, true);
                    Console.WriteLine("Installed schema file: " + text2);
                }
                else
                {
                    Console.WriteLine("Schema file is already in the destination location.");
                }
            }
            finally
            {
                if (IntPtr.Zero != zero)
                {
                    Wow64Redirection.Wow64RevertWow64FsRedirection(zero);
                }
            }
            SectionList configurationSections = GetConfigurationSections(text2);
            RegisterConfigurationSections(configurationSections, false);
            Console.WriteLine("Finished");
        }

        /* add specific cleanup logic here (basically mirror install)
         * No need to clean Program Files or other aspects of install, just revert the things you manually modified in Install() */
        public void Uninstall()
        {
        }

        private SectionList GetConfigurationSections(string schemaPath)
        {
            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.Load(schemaPath);
            SectionList sectionList = new SectionList();
            XmlElement documentElement = xmlDocument.DocumentElement;
            foreach (XmlNode xmlNode in documentElement.ChildNodes)
            {
                if (xmlNode.LocalName.Equals("sectionSchema", StringComparison.InvariantCultureIgnoreCase))
                {
                    sectionList.Add(new SectionDefinition(xmlNode.Attributes["name"].Value));
                }
            }
            return sectionList;
        }

        private static SectionList GetConfigurationSections(string sectionGroupName, IAppHostSectionGroup sectionGroup)
        {
            SectionList sectionList = new SectionList();
            string text = string.IsNullOrEmpty(sectionGroupName) ? sectionGroup.Name : (sectionGroupName + "/" + sectionGroup.Name);
            for (uint num = 0u; num < sectionGroup.Count; num += 1u)
            {
                IAppHostSectionGroup sectionGroup2 = sectionGroup[num];
                SectionList configurationSections = GetConfigurationSections(text, sectionGroup2);
                sectionList.AddRange(configurationSections);
            }
            IAppHostSectionDefinitionCollection sections = sectionGroup.Sections;
            for (uint num2 = 0u; num2 < sections.Count; num2 += 1u)
            {
                IAppHostSectionDefinition appHostSectionDefinition = sections[num2];
                sectionList.Add(new SectionDefinition(string.IsNullOrEmpty(text) ? appHostSectionDefinition.Name : (text + "/" + appHostSectionDefinition.Name), (AllowDefinition)Enum.Parse(typeof(AllowDefinition), appHostSectionDefinition.AllowDefinition, true), (OverrideModeDefault)Enum.Parse(typeof(OverrideModeDefault), appHostSectionDefinition.OverrideModeDefault, true), appHostSectionDefinition.Type));
            }
            return sectionList;
        }

        private static bool RegisterConfigSection(IAppHostWritableAdminManager adminManager, SectionDefinition sectionDefinition, string sectionName, IAppHostSectionGroup sectionGroup, bool remove)
        {
            string text = null;
            string sectionName2 = sectionName;
            int num = sectionName.IndexOf('/');
            if (num >= 0)
            {
                text = sectionName.Substring(0, num);
                sectionName2 = sectionName.Substring(num + 1, sectionName.Length - num - 1);
            }
            if (text != null)
            {
                uint count = sectionGroup.Count;
                for (uint num2 = 0u; num2 < count; num2 += 1u)
                {
                    IAppHostSectionGroup appHostSectionGroup = sectionGroup[num2];
                    if (appHostSectionGroup.Name.Equals(text, StringComparison.InvariantCultureIgnoreCase))
                    {
                        return RegisterConfigSection(adminManager, sectionDefinition, sectionName2, appHostSectionGroup, remove);
                    }
                }
                if (remove)
                {
                    return false;
                }
                IAppHostSectionGroup sectionGroup2 = sectionGroup.AddSectionGroup(text);
                return RegisterConfigSection(adminManager, sectionDefinition, sectionName2, sectionGroup2, remove);
            }
            else
            {
                IAppHostSectionDefinitionCollection sections = sectionGroup.Sections;
                bool flag = false;
                uint count2 = sections.Count;
                for (uint num3 = 0u; num3 < count2; num3 += 1u)
                {
                    IAppHostSectionDefinition appHostSectionDefinition = sections[num3];
                    if (appHostSectionDefinition.Name.Equals(sectionName, StringComparison.InvariantCultureIgnoreCase))
                    {
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    if (remove)
                    {
                        return false;
                    }
                    IAppHostSectionDefinition appHostSectionDefinition2 = sections.AddSection(sectionName);
                    appHostSectionDefinition2.OverrideModeDefault = sectionDefinition.OverrideModeDefault.ToString();
                    appHostSectionDefinition2.AllowDefinition = sectionDefinition.AllowDefinition.ToString();
                    appHostSectionDefinition2.Type = sectionDefinition.Type;
                    return true;
                }
                else
                {
                    if (remove)
                    {
                        try
                        {
                            IAppHostElement adminSection = adminManager.GetAdminSection(sectionName, "MACHINE/WEBROOT/APPHOST");
                            adminSection.Clear();
                        }
                        catch (Exception)
                        {
                        }
                        sections.DeleteSection(sectionName);
                        return true;
                    }
                    return false;
                }
            }
        }

        private void RegisterConfigurationSections(SectionList sectionList, bool remove)
        {
            IAppHostWritableAdminManager appHostWritableAdminManager = new AppHostWritableAdminManagerClass();
            appHostWritableAdminManager.CommitPath = "MACHINE/WEBROOT/APPHOST";
            IAppHostConfigManager configManager = appHostWritableAdminManager.ConfigManager;
            IAppHostConfigFile configFile = configManager.GetConfigFile("MACHINE/WEBROOT/APPHOST");
            IAppHostSectionGroup rootSectionGroup = configFile.RootSectionGroup;
            foreach (SectionDefinition current in sectionList)
            {
                if (RegisterConfigSection(appHostWritableAdminManager, current, current.Name, rootSectionGroup, remove))
                {
                    if (remove)
                    {
                        Console.WriteLine("Unregistered section: " + current.Name);
                    }
                    else
                    {
                        Console.WriteLine("Registered section: " + current.Name);
                    }
                }
                else
                {
                    if (remove)
                    {
                        Console.WriteLine("Section not currently registered, ignoring: " + current.Name);
                    }
                    else
                    {
                        Console.WriteLine("Section already registered, ignoring: " + current.Name);
                    }
                }
            }
            appHostWritableAdminManager.CommitChanges();
        }

        internal class Wow64Redirection
        {
            [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
            public static extern bool IsWow64Process(IntPtr hProcess, out bool isWow64);
            [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
            public static extern bool Wow64DisableWow64FsRedirection(out IntPtr oldValue);
            [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
            public static extern bool Wow64RevertWow64FsRedirection(IntPtr oldValue);
        }

        private void CopyBinaries(string srcpath, string dstpath)
        {
            string[] files = { "libapr-1.dll", "libapriconv-1.dll", "libaprutil-1.dll", 
                               "libxml2.dll", "lua5.1.dll", "pcre.dll", "zlib1.dll", "ModSecurityIIS.dll" };

            foreach (string s in files)
                File.Copy(Path.Combine(srcpath, s), Path.Combine(dstpath, s), true);
        }

        public enum AllowDefinition
        {
            MachineOnly,
            MachineToWebRoot,
            MachineToApplication,
            AppHostOnly,
            Everywhere
        }

        public enum OverrideModeDefault
        {
            Allow,
            Deny
        }

        public class SectionDefinition
        {
            public const AllowDefinition DefaultAllowDefinition = AllowDefinition.Everywhere;
            public const OverrideModeDefault DefaultOverrideModeDefault = OverrideModeDefault.Allow;
            private string name;
            private AllowDefinition allowDefinition;
            private OverrideModeDefault overrideModeDefault;
            private string type;
            public string Name
            {
                get
                {
                    return this.name;
                }
            }
            public AllowDefinition AllowDefinition
            {
                get
                {
                    return this.allowDefinition;
                }
            }
            public OverrideModeDefault OverrideModeDefault
            {
                get
                {
                    return this.overrideModeDefault;
                }
            }
            public string Type
            {
                get
                {
                    return this.type;
                }
                set
                {
                    this.type = value;
                }
            }
            public SectionDefinition(string sectionName)
                : this(sectionName, AllowDefinition.Everywhere, OverrideModeDefault.Allow, null)
            {
            }
            public SectionDefinition(string sectionName, AllowDefinition allowDefinition, OverrideModeDefault overrideModeDefault, string type)
            {
                this.name = sectionName;
                this.allowDefinition = allowDefinition;
                this.overrideModeDefault = overrideModeDefault;
                this.type = type;
            }
        }

        internal class SectionList : List<SectionDefinition> {}
    }
}
