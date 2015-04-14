class Distance {


private:
	static double radian(double d);
public:

	/**
	 * 给定两点的经纬度，返回两点的欧氏距离
	 */
	static double getDist(double lat1, double lon1, double lat2, double lon2);
};