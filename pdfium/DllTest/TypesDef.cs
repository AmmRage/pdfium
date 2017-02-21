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
    #region type
    [StructLayout(LayoutKind.Sequential)]
    public struct Bound
    {
        public double Left;
        public double Top;
        public double Right;
        public double Bottom;


        public Bound(double left, double top, double right, double bottom)
        {
            this.Left = left;
            this.Top = top;
            this.Right = right;
            this.Bottom = bottom;
        }

        public override string ToString()
        {
            return String.Format("Left: {0}, Top: {1}, Right: {2}, Bottom: {3}", this.Left, this.Top, this.Right, this.Bottom);
        }
    }
    [StructLayout(LayoutKind.Sequential)]
    [Serializable]
    public struct InputOutput
    {
        public int PageCount;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
        public char[] filename;//[256];

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 250)]
        public char[] capture;//[50];

        [MarshalAs(UnmanagedType.U1, SizeConst = 1)]
        public byte IfCaptured;

        public InputOutput(char[] filename)
        {
            this.filename = new char[256];
            Array.Copy(filename, this.filename, filename.Length);
            this.capture = new char[250];
            this.IfCaptured = 0;
            this.PageCount = 0;
        }

        public override string ToString()
        {
            //return string.Format("{0}{1}{2}{3}{4}{5}",
            return string.Format("{0}, {1}, {2}, {3}, {4}, {5}",
            "",//new string(this.filename, 0, GetCharValidLength(ref this.filename, 0, 256)),
                new string(this.capture, 0, GetCharValidLength(ref this.capture,0, 50)).Trim(),
                new string(this.capture, 50, GetCharValidLength(ref this.capture, 50, 50)).Trim(),
                new string(this.capture, 100, GetCharValidLength(ref this.capture, 100, 50)).Trim(),
                new string(this.capture, 150, GetCharValidLength(ref this.capture, 150, 50)).Trim(),
                new string(this.capture, 200, GetCharValidLength(ref this.capture, 200, 50)).Trim());
        }

        private static int GetCharValidLength(ref char[] arr, int offset, int length)
        {
            for (var i = offset + length - 1; i >= offset; i --)
            {
                if (arr[i] == 0)
                    continue;
                else
                    return i - offset + 1;
            }
            return 0;
        }

    };
    [StructLayout(LayoutKind.Sequential)]
    public struct ProcessInfo
    {
        public string filename;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40002)]
        public byte[] buffer;
        public int length;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
        public int[] textLengths;

        //public ProcessInfo(
        //    object buffer,
        //    object length,
        //    //object bounds,
        //    //object boundsCount,
        //    object textLengths)
        //{
        //    this.buffer = (byte[])buffer;
        //    this.length = (int)length;
        //    //this.bounds = (Bound[])bounds;
        //    //this.boundsCount = (int)boundsCount;
        //    this.textLengths = (int[])textLengths;
        //}
        public ProcessInfo(string filename,
            byte[] buffer,
            int length,
            //Bound[] bounds,
            //int boundsCount,
            int[] textLengths)
        {
            this.filename = filename;
            this.buffer = buffer;
            this.length = length;
            //this.bounds = bounds;
            //this.boundsCount = boundsCount;
            this.textLengths = textLengths;
        }
        //public ProcessInfo(ProcessInfo pi)
        //{
        //    this.buffer = pi.buffer;
        //    this.length = pi.length;
        //    //this.bounds = pi.bounds;
        //    //this.boundsCount = pi.boundsCount;
        //    this.textLengths = pi.textLengths;
        //}
    }
    #endregion

}