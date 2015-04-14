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
        /**
         * 读取osm文件中所有道路信息，将实际路网中的way按照node切分成图中的边
         */
        private static void getAllWays() { 

            FileStream wayStream  = new FileStream("ways.txt", FileMode.Create);
            StreamWriter wayFile  = new StreamWriter(wayStream, Encoding.Default);
            int allRoads    = 0;
            try
            {
                XmlDocument doc = new XmlDocument();
                doc.Load("planet_121.072,30.728_122.298,31.421.osm");
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
                            wayFile.WriteLine(last + " " + now);
                            last = now;
                            allRoads++;
                        }
                        last = now;
                    }
                }
            } catch (Exception e) {
                Console.WriteLine(e);
                wayFile.Close();
                wayStream.Close();
                Console.ReadLine();
            }
            wayFile.Close();
            wayStream.Close();
            Console.WriteLine(allRoads);
       }

        /**
         * 读取路网中所有的Node的信息
         */ 
        private static void getAllNodes() {
            FileStream nodeStream = new FileStream("nodes.txt", FileMode.Create);
            StreamWriter nodeFile = new StreamWriter(nodeStream, Encoding.Default);
            int allNodes = 0;
            try
            {
                XmlDocument doc = new XmlDocument();
                doc.Load("planet_121.072,30.728_122.298,31.421.osm");
                XmlElement root = doc.DocumentElement;
                XmlNodeList nodeList = root.GetElementsByTagName("node");
                foreach (XmlElement node in nodeList)
                {
                    string id  = node.GetAttribute("id");
                    string lat = node.GetAttribute("lat");
                    string lon = node.GetAttribute("lon");
                    nodeFile.WriteLine(id + " " + lat + " " + lon);
                    allNodes++;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                nodeFile.Close();
                nodeStream.Close();
                Console.ReadLine();
            }
            nodeFile.Close();
            nodeStream.Close();
            Console.WriteLine(allNodes);
            Console.ReadLine();

        }

        static void Main(string[] args)
        {
            getAllWays();
            getAllNodes();
        }
            
    }
}
