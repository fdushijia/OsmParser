OSM是Open Street Map的简称，http://wiki.openstreetmap.org/wiki/Main_Page

后缀名为.osm的文件是一种xml格式存储的文件，存储了道路网络上的数据。

.osm文件中最重要的两种结构是Node和Way。存储结构如下：

<osm version="0.6" generator="OpenStreetMap server"> 

    <node id="483034256" lat="55.9458449" lon="-3.2035477" version="1" 

        changeset="2369219" user="spytfyre" uid="166957" visible="true" 

        timestamp="2009-09-04T13:35:42Z"> 

        <tag k="name" v="The Blue Blazer" /> 

        <tag k="amenity" v="pub" /> 

    </node> 
    
    <way id="43157302" visible="true" timestamp="2009-10-26T10:45:09Z" 

        version="1" changeset="2954960" user="Ed Avis" uid="31257"> 

        <nd ref="540653724" /> 

        <nd ref="25507043" /> 

        <nd ref="107762" /> 

        <nd ref="25507038" /> 

        <nd ref="107759" /> 

        <tag k="highway" v="primary" /> 

        <tag k="lcn_ref" v="6a" /> 

        <tag k="name" v="Parliament Street" /> 

    </way> 
</osm>

OsmParser要将.osm文件所表示的道路数据解析并存储为图文件。目前完成了获取路网中所有的边。
