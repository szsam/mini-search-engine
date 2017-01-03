package http.demo;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;

class Result {
	public String url;		//url地址
	public int index;		//网页编号（预先已编号）
	public double pagerank;	//pagerank排序
	public int degreerank;	//入度值
	public Result(int index) {
		this.index = index;
	}
}

public class Search {
	//这两个为cpp实现的函数，通过加载Search动态库来调用
	private native void initData();
	private native Result[] searchResults(String[] keyArray);
	static {
		System.loadLibrary("Search");
	}
	private ArrayList<String> urls= new ArrayList<String>();
	private ArrayList<Double> pageranks = new ArrayList<Double>();
	private ArrayList<Integer> degreeranks = new ArrayList<Integer>();
	public Search() throws IOException {
		//cpp函数，初始化倒排索引信息
		initData();
		//读入url, pagerank, degreerank信息
		BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(
				"html" + File.separator + "pagelist.txt"), "gbk"));
		String line;
		while ((line = reader.readLine()) != null) {
			int beg = line.indexOf("://") + "://".length();
			if (beg < "://".length()) { beg = 0; }
			urls.add(line.substring(beg));
		}
		reader.close();
		reader = new BufferedReader(new InputStreamReader(new FileInputStream(
				"html" + File.separator + "pagerank.txt"), "gbk"));
		while ((line = reader.readLine()) != null) {
			pageranks.add(new Double(line));
		}
		reader.close();
		reader = new BufferedReader(new InputStreamReader(new FileInputStream(
				"html" + File.separator + "degreerank.txt"), "gbk"));
		while ((line = reader.readLine()) != null) {
			degreeranks.add(new Integer(line));
		}
		reader.close();
	}
	public ArrayList<Result> search(String[] keyArray) {
		ArrayList<Result> results = new ArrayList<Result>();
		//调用cpp函数进行搜索，此接口包含了布尔检索的功能
		Result[] results2 = searchResults(keyArray);
		for (int i = 0; i < results2.length; i ++) {
			try {
				//设置相关信息
				results2[i].url = urls.get(results2[i].index);
				results2[i].pagerank = pageranks.get(results2[i].index);
				results2[i].degreerank = degreeranks.get(results2[i].index);
				results.add(results2[i]);
			}
			catch (Exception e) {
				continue;
			}
		}
		//进行排序，根据pagerank值
		results.sort(new Comparator<Result>() {
			@Override
			public int compare(Result o1, Result o2) {
				if (o1.pagerank == o2.pagerank) { return 0; }
				if (o1.pagerank > o2.pagerank) { return -1; }
				return 1;
			}
		});
		return results;
	}
}
