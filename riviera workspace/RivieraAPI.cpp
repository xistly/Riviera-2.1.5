using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using FastColoredTextBoxNS;

namespace Riviera_fix_2._0._2
{
    public partial class Riviera : Form
    {
        public Point mouseLocation;

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr OpenProcess(uint processAccess, bool bInheritHandle, int processId);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lpModuleName);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out IntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);

        private const uint PROCESS_ALL_ACCESS = 0x1F0FFF;
        private const uint MEM_COMMIT = 0x1000;
        private const uint PAGE_READWRITE = 0x04;

        public Riviera()
        {
            InitializeComponent();

            // Attach the event handler to the CheckBox's CheckedChanged event
            checkBox1.CheckedChanged += checkBox1_CheckedChanged;
        }

        private void InjectButton_Click(object sender, EventArgs e)
        {
            if (!IsProcessRunning("RobloxPlayer"))
            {
                MessageBox.Show("Roblox is not running!");
                return;
            }

            try
            {
                InjectDLL("RobloxPlayer", "path_to_your_dll.dll");
                ExecuteRobloxCode("print(\"Riviera ontop\")");
                MessageBox.Show("Injected successfully!");
            }
            catch
            {
                MessageBox.Show("Failed injecting");
            }
        }

        private void ExecuteButton_Click(object sender, EventArgs e)
        {
            if (IsInjected())
            {
                ExecuteRobloxCode(fastColoredTextBox1.Text);
            }
            else
            {
                MessageBox.Show("Please inject before executing!");
            }
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            fastColoredTextBox1.Clear();
        }

        private void LoadFileButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog
            {
                Title = "Open"
            };

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                fastColoredTextBox1.Text = File.ReadAllText(openFileDialog1.FileName);
            }
        }

        private void SaveFileButton_Click(object sender, EventArgs e)
        {
            string scriptsFolder = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Scripts");

            SaveFileDialog saveFileDialog1 = new SaveFileDialog
            {
                Filter = "Lua Files (*.lua)|*.lua|All Files (*.*)|*.*",
                DefaultExt = "lua",
                AddExtension = true,
                InitialDirectory = scriptsFolder
            };

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if (!Directory.Exists(scriptsFolder))
                {
                    Directory.CreateDirectory(scriptsFolder);
                }

                string filePath = saveFileDialog1.FileName;

                using (Stream s = File.Open(filePath, FileMode.Create))
                using (StreamWriter sw = new StreamWriter(s))
                {
                    sw.Write(fastColoredTextBox1.Text);
                }
            }
        }

        private bool IsProcessRunning(string processName)
        {
            return Process.GetProcessesByName(processName).Length > 0;
        }

        private void InjectDLL(string processName, string dllPath)
        {
            Process[] processes = Process.GetProcessesByName(processName);
            if (processes.Length == 0)
                throw new Exception("Process not found");

            Process process = processes[0];
            IntPtr hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, process.Id);

            IntPtr loadLibraryAddr = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
            IntPtr allocMemAddress = VirtualAllocEx(hProcess, IntPtr.Zero, (uint)((dllPath.Length + 1) * Marshal.SizeOf(typeof(char))), MEM_COMMIT, PAGE_READWRITE);

            WriteProcessMemory(hProcess, allocMemAddress, Encoding.Default.GetBytes(dllPath), (uint)((dllPath.Length + 1) * Marshal.SizeOf(typeof(char))), out _);
            CreateRemoteThread(hProcess, IntPtr.Zero, 0, loadLibraryAddr, allocMemAddress, 0, IntPtr.Zero);
        }

        private void ExecuteRobloxCode(string code)
        {
            // Implement your logic to execute code in Roblox
        }

        private bool IsInjected()
        {
            // Implement your logic to check if the code is injected
            return true; // Placeholder
        }

        private void mouse_Down(object sender, MouseEventArgs e)
        {
            mouseLocation = new Point(-e.X, -e.Y);
        }

        private void mouse_Move(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Point mousePose = Control.MousePosition;
                mousePose.Offset(mouseLocation.X, mouseLocation.Y);
                Location = mousePose;
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            WindowState = FormWindowState.Minimized;
        }

        private void Riviera_Load(object sender, EventArgs e)
        {
            TopMost = true;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            // Implement your logic for the CheckBox's CheckedChanged event
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Implement your logic for the ListBox's SelectedIndexChanged event
        }
    }
}
