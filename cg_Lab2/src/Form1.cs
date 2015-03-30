using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Imaging;


namespace cg_Lab2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            updatePicture("image.jpg");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                updatePicture(openFileDialog1.FileName);
        }

        private void updatePicture(String fileName)
        {
            Image image;
            try
            {
                image = Image.FromFile(fileName);
            }
            catch (System.IO.FileNotFoundException)
            {
                return;
            }

            pictureBox1.Image = image;

            PropertyItem[] propItems = image.PropertyItems;

            txtSize.Text = image.PhysicalDimension.Width.ToString() + " * " + image.PhysicalDimension.Height.ToString();
            txtDPI.Text = image.HorizontalResolution.ToString();

            txtCompression.Text = "no data";
            foreach (PropertyItem i in propItems)
                if (i.Id == 0x0103)
                    txtCompression.Text = i.Value.ToString();

            txtLuminanceTable.Text = "";
            foreach (PropertyItem property in image.PropertyItems)
                if (property.Id == 0x5090)
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                            txtLuminanceTable.Text += String.Format("{0,4:X} ", property.Value[i * 16 + j]);
                        txtLuminanceTable.Text += Environment.NewLine;
                    }

            txtChrominanceTable.Text = "";
            foreach (PropertyItem property in image.PropertyItems)
                if (property.Id == 0x5091)
                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 8; j++)
                            txtChrominanceTable.Text += String.Format("{0,4:X} ", property.Value[i * 16 + j]);
                        txtChrominanceTable.Text += Environment.NewLine;
                    }
        }
    }
}
