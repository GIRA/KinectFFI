using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
          

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Image.setHue(220, 260);
            Image.setSaturation(0.5, 1.5);
            Image.setBrightness(0.5, 1.5);
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Bitmap bmp = (Bitmap)Bitmap.FromFile(openFileDialog1.FileName);
                System.Drawing.Imaging.BitmapData data = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format16bppArgb1555);
                int size = Math.Abs(data.Stride) * bmp.Height;
                byte[] bytes = new byte[size];

                System.Runtime.InteropServices.Marshal.Copy(data.Scan0, bytes, 0, size);
                Image.setRectangleOverlapping(50);
                ushort[] bits = new ushort[size ];
                Rect r;
                System.Buffer.BlockCopy(bytes, 0, bits, 0, size);
                unsafe
                {
                    fixed (ushort* p = &bits[0])
                    {
                       r= Image.trackMainRectangle(p, bmp.Width, bmp.Height);
                    }
                }
                bmp.UnlockBits(data);
 
            }
        }
    }
}
