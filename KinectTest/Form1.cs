using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KinectTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           Console.WriteLine( KinectDll.getKinectCount());
           KinectDll.initializeOn(0);

            float[] f = new float[500];
            int count=0;
            System.Threading.Thread th  = new System.Threading.Thread(new System.Threading.ParameterizedThreadStart(delegate {
            while (true)
            {
                f[0] = KinectDll.getNextFrame();
                if(count !=0) Console.WriteLine(f[0] + " : " + f[1]);
                System.Threading.Thread.Sleep(100);
            }}));
            th.Start();
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            KinectDll.uninitialize();
        }
    }
}
