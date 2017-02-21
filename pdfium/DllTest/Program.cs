using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DllTest
{
    class Program
    {
        #region attributes
        static Bound[] bounds = new[]
        {
            new Bound(371.0, 587.0, 548.0, 531.0),
            new Bound(253.0, 499.0, 325.0, 485.0),
            new Bound(327.0, 500.0, 398.0, 485.0),
            new Bound(401.0, 500.0, 471.0, 484.0),
            new Bound(474.0, 500.0, 543.0, 484.0),
        };
        private static readonly Encoding FPDFEncoding = new UnicodeEncoding(false, false, false);
        #endregion

        static void Main(string[] args)
        {
            Console.WriteLine("Press anykey to sart....");
            Console.ReadKey();
            //var filename = @"C:\TEST\TestFolder_CPF\YSOA PDF checking\profile\YSOA_2016_000BE342-S215-2446-83E9-6FFCC145777C.pdf";
            //try
            //{
            //    if (!TestDllWork())
            //    {
            //        Console.WriteLine("Error");
            //        return;
            //    }
            //}
            //catch (Exception ex)
            //{
            //    Console.WriteLine(ex);
            //    return;
            //}
            TestNewPara();
            //TestMemleak();
            //TestMultiThreadCall();
            //TestModStructArr();
            //InitLibrary();
            //TestSingle(filename);
            //TestMulti(filename);
            //DestroyLibrary();
        }

        static void TestMulti(string filename)
        {
            var getLength = 65535;
            var result = new byte[(getLength + 1) * 2];
            var readLengths = new int[5];
            var len = GetMultiBoundedText(filename.ToCharArray(), result, getLength,
                new []
                {
                    //index 1, 
                    new Bound(360, 841- 252, 552,841 - 313),
                    //index 2, 
                    new Bound(253, 841 - 342, 325, 841-356 ),
                    //index 3, 
                    new Bound(327, 841-341, 398,841- 356),
                    //index 5, 
                    new Bound(401,841- 341, 471,841- 357),
                    //index 6, 
                    new Bound(474,841- 341, 543,841- 357),
                },
                5,readLengths);

            var index = 0;
            for (var i = 0; i < readLengths.Length; i++)
            {
                if (readLengths[i] == 0)
                    continue;//Console.WriteLine("Emplty");
                var text = FPDFEncoding.GetString(result, index, readLengths[i] * 2);
                //Trace.WriteLine(text);
                index += readLengths[i]*2;
            }
        }
        static void TestModStructArr()
        {
            var bounds = new[]
            {
                 new Bound(0,0,0,0),
                 new Bound(1,0,0,0),
                 new Bound(2,0,0,0),
            };

            Trace.WriteLine(bounds.Length);
            ModifyStructValue(bounds, 3);
            foreach (var bound in bounds)
            {
                Trace.WriteLine(bound);
            }
        }
        static void TestMemleak()
        {
            var memStatList = new List<string>();
            var random = new Random(Environment.TickCount);
            var count = 10000;
            var folder = @"C:\TEST\TestFolder_CPF\YSOA PDF checking\profile";            
            var para = new DirectoryInfo(folder).GetFiles().Take(count).Select(f => new InputOutput(f.FullName.ToCharArray())).ToArray();
            Enumerable.Range(0, count).ToList().ForEach(i =>
            {
                InitLibrary();
                SetCaptureBounds(bounds, 5);
                GetOutputString(new[] { para[i]}, 1);

                Thread.Sleep(random.Next(0, 5) * 10);
                Console.WriteLine("Run time: " + i);
                GC.Collect();
                memStatList.Add(Process.GetCurrentProcess().PrivateMemorySize64.ToString());
                DestroyLibrary();
            });
            File.WriteAllLines("memstat.txt", memStatList.ToArray());
            Console.WriteLine("Done");
        }
        static void TestSingle(string filename)
        {
            var getLength = 65535;
            var result = new byte[(getLength + 1) * 2];
            var len = GetBoundedText(filename.ToCharArray(), result, getLength);
            var text = FPDFEncoding.GetString(result, 0, len * 2);
            Console.WriteLine(text);
        }
        static void TestMultiThreadCall()
        {
            var folder = @"C:\TEST\TestFolder_CPF\YSOA PDF checking\PdfPerformance";


            var results = new DirectoryInfo(folder).GetFiles()
                .Take(1)
                .Select(f =>
                {
                    var getLength = 20000;
                    var result = new byte[(getLength + 1)*2];
                    var readLengths = new int[5];
                    //var ret = "";
                    //var buf = File.ReadAllBytes(f.FullName);

                    //return f.FullName;
                    var pInfo = new ProcessInfo(f.FullName,  result, getLength,  readLengths);
                    //var index = 0;
                    //for (var i = 0; i < readLengths.Length; i++)
                    //{
                    //    if (readLengths[i] == 0)
                    //        continue;//Console.WriteLine("Emplty");
                    //    //var text = FPDFEncoding.GetString(result, index, readLengths[i] * 2);
                    //    ret += FPDFEncoding.GetString(result, index, readLengths[i] * 2);
                    //    index += readLengths[i] * 2;
                    //}
                    return pInfo;
                    //return new object[] {f.FullName.ToCharArray(), result, getLength, bounds, 5, readLengths};
                }).ToArray();
            //for (var i = 0; i < 50; i++)
            //{
            //    processInfos[i] = new ProcessInfo(results.Current);
            //    results.MoveNext();
            //}
            //processInfos[0] = new ProcessInfo(null, null, 0, null, 0, null);
            //processInfos[0] = new ProcessInfo(results[0]);
            //processInfos[0] = new ProcessInfo(results[0][0],
            //    results[0][1],
            //    results[0][2],
            //    results[0][3],
            //    results[0][4],
            //    results[0][5]);

            //var size= Marshal.SizeOf(results[0]);
            //Trace.WriteLine(size);
            InitLibrary();
            //Imported_MultiThreadPoolProcess(processInfos, processInfos.Length);
            SetCaptureBounds(bounds, 5);
            Imported_MultiThreadPoolProcess(results, results.Length);


            var texts = results.Select(r =>
            {
                var index = 0;
                var text = "";

                for (var i = 0; i < r.textLengths.Length; i++)
                {
                    if (r.textLengths[i] == 0)
                        continue;//Console.WriteLine("Emplty");
                    text += FPDFEncoding.GetString(r.buffer, index, r.textLengths[i] * 2);
                    //Trace.WriteLine(text);
                    index += r.textLengths[i] * 2;
                }
                Debug.WriteLine(text);
                return text;
            }).ToList();


            DestroyLibrary();
            Trace.WriteLine(results.Length);
        }

        static void TestNewPara()
        {
            Console.WriteLine();
            var folder = @"C:\TEST\TestFolder_CPF\YSOA PDF checking\test_runable";
            //var folder = @"C:\TEST\TestFolder_CPF\YSOA PDF checking\profile";
            var para = new DirectoryInfo(folder).GetFiles().Select(f => new InputOutput(f.FullName.ToCharArray())).ToArray();
            InitLibrary();
            SetCaptureBounds(bounds, 5);
            GetOutputString(para, para.Length);
            foreach (var p in para)
            {
                Trace.WriteLine(p);
            }
            DestroyLibrary();
        }

        #region static import

        [DllImport("pdfium_test.dll")]
        public static extern int GetBoundedText(char[] filename, byte[] buffer, int length);

        [DllImport("pdfium_test.dll")]
        public static extern int GetMultiBoundedText(char[] filename, byte[] buffer, int length, 
            Bound[] bounds, int boundsCount, int[] textLengths );

        [DllImport("pdfium_test.dll")]
        public static extern bool TestDllWork();

        [DllImport("pdfium_test.dll")]
        public static extern bool InitLibrary();
         
        [DllImport("pdfium_test.dll")]
        public static extern bool DestroyLibrary();

        [DllImport("pdfium_test.dll")]
        public static extern void ModifyStructValue([In, Out] Bound[] bounds, int length);

        [DllImport("pdfium_test.dll")]
        public static extern void SetCaptureBounds([In, Out] Bound[] bounds, int length);

        [DllImport("pdfium_test.dll", EntryPoint = "GetMultiBoundedText")]
        public static extern int Imported_GetMultiBoundedText(char[] filename, byte[] buffer, int length, Bound[] bounds, int boundsCount, int[] textLengths);

        [DllImport("pdfium_test.dll", EntryPoint = "MultiThreadPoolProcess", CallingConvention = CallingConvention.Cdecl)]
        //public static extern int Imported_MultiThreadPoolProcess([MarshalAs( UnmanagedType.LPArray)] [In, Out] ProcessInfo[] processInfos, int length);
        //public static extern int Imported_MultiThreadPoolProcess([In]string[] filename, [In, Out] ProcessInfo[] processInfos, int length);
        public static extern int Imported_MultiThreadPoolProcess( [In, Out] ProcessInfo[] processInfos, int length);

        [DllImport("pdfium_test.dll", EntryPoint = "TestConvert")]
        public static extern void TestConvert([In] byte[] textBuf, [Out] char[] output);

        [DllImport("pdfium_test.dll", EntryPoint = "GetOutputString")]
        public static extern void GetOutputString([In, Out] InputOutput[] para, int length);
        #endregion
    }
}