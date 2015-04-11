using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using System.Xml;
using System.IO;

namespace ParseOsm
{
    class Program
    {
        static void Main(string[] args)
        {
            FileStream fs   = new FileStream("ways.txt", FileMode.Create);
            StreamWriter sw = new StreamWriter(fs, Encoding.Default);
            int allRoads    = 0;
            try
            {
                XmlDocument doc = new XmlDocument();
                doc.Load("test.osm");
                XmlElement root = doc.DocumentElement;
                XmlNodeList wayList = root.GetElementsByTagName("way");
                foreach (XmlElement way in wayList)
                {
                    string last = "";
                    XmlNodeList nodes = way.GetElementsByTagName("nd");
                    foreach (XmlElement node in nodes)
                    {
                        string now = node.GetAttribute("ref");
                        if (last != "")
                        {
                            sw.WriteLine(last + " " + now);
                            last = now;
                            allRoads++;
                        }
                        last = now;
                    }
                }
            }
            catch (Exception e) {
                Console.WriteLine(e);
                sw.Close();
                fs.Close();
                Console.ReadLine();
            }
            sw.Close();
            fs.Close();
            Console.WriteLine(allRoads);
            Console.ReadLine();
        }
    }
}
