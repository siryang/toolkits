using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 传票练习系统1._1
{
   
    public partial class Form1 : Form
    {
        public static Float_s  array;
        public float result;
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
           private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {

            string del = ".88";
            string cal = ".99";
            if (e.KeyCode == Keys.Up)
            {
                this.textBox1.Clear();
            }
            if (e.KeyCode == Keys.Enter)
            {
                float num = float.Parse(this.textBox1.Text);
                if (this.textBox1.Text == del)
                {
                    array.Del_Num();

                }
                else if (this.textBox1.Text == cal)
                {
                    result = array.Cal_Result();
                    Update_Result(result);
                }else{
                    array.Add_Num(num);
             
                }
                this.textBox1.Clear();
            }
        }

        private void Update_Result(float num)
        {
            string str = num.ToString();
            this.textBox2.Text = str;
        }
    }
    public class Float_s
    {
        public float[] array;
        private int index;
        public float result;
        public Float_s()
        {
            this.array = new float[200];
            for (int i = 0; i < 200; i++)
            {
                this.array[i] = 0;
            }
            index = 0;
        }
        public void Del_Num()
        {
            index--;
            try
            {
                array[index] = 0;
            }
            catch
            {
                Console.WriteLine("Error number");
            }
            return;
        }
        public void Add_Num(float num)
        {
            try
            {
                array[index] = num;
            }
            catch
            {
                Console.WriteLine("Error number");
            }
            index++;
            return;

        }
        public float Cal_Result()
        {
            int i = 0;
            float result = 0;
            for (i = 0; i < index; i++)
            {
                try
                {
                    result = result + array[i];
                }
                catch
                {
                    Console.WriteLine("Error number");
                }
            }
            return result;
        }
    }
 
}
