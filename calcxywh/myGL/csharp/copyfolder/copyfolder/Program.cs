using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace copyfolder
{
    class Program
    {
        static void ShowDir(string dir)
        {
            DirectoryInfo dinfo = new DirectoryInfo(dir);
            FileInfo[] files = dinfo.GetFiles();
            foreach (FileInfo file in files)
            {
                Console.WriteLine(file.ToString());
            }
            DirectoryInfo[] dirs = dinfo.GetDirectories();
            foreach (DirectoryInfo d in dirs)
            {
                Console.WriteLine("directory:" + d.ToString());
            }
        }
        static void CopyDir(string dir, string dst)
        {
            //#if 0
            //            DirectoryInfo DstDir = Directory.CreateDirectory(dst);
            //            DirectoryInfo dinfo = new DirectoryInfo(dir);
            //            FileInfo[] files = dinfo.GetFiles();
            //            foreach(FileInfo file in files)
            //            {
            //                file.CopyTo(DstDir.FullName + "\\" + file.ToString(), true);
            //            }
            //            DirectoryInfo[] dirs = dinfo.GetDirectories();
            //            foreach (DirectoryInfo dd in dirs)
            //            {
            //                Directory.CreateDirectory(DstDir.FullName + "\\" + dd.ToString());
            //            }
            //#else
            CopyDirRecursive(new DirectoryInfo(dir), dst);
            //#endif
        }
        static void CopyDirRecursive(DirectoryInfo dinfo, String dst)
        {
            // 过滤掉svn
            // 只检索cpp，h文件
            DirectoryInfo[] dirs = dinfo.GetDirectories();
            foreach (DirectoryInfo dd in dirs)
            {
                if (dd.ToString() == ".svn")
                {
                    continue;
                }
                string ddst = dst + "\\" + dd.ToString();
                CopyDirRecursive(dd, ddst);
            }

            FileInfo[] files = dinfo.GetFiles();
            foreach (FileInfo file in files)
            {
                // 过滤掉一些文件
                bool docopy = false;
                string[] filters = new string[] { ".cpp", ".h", ".inl", ".uc", ".vcproj", ".sln" };
                foreach (string cpp in filters)
                {
                    if (file.ToString().EndsWith(cpp, StringComparison.CurrentCultureIgnoreCase))
                    {
                        docopy = true;
                        break;
                    }
                }
                // 拷贝文件
                if (docopy)
                {
                    if (!Directory.Exists(dst))
                    {
                        Directory.CreateDirectory(dst);
                    }
                    file.CopyTo(dst.ToString() + "\\" + file.ToString(), true);
                }
            }

        }
        static void Main(string[] args)
        {
            Console.WriteLine("hello world");
            //ShowDir("J:\\dh\\engine");
            CopyDir("J:\\dh\\engine", "J:\\engine");
        }
    }
}
