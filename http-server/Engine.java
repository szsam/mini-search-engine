package http.demo;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import com.huaban.analysis.jieba.JiebaSegmenter;
import com.huaban.analysis.jieba.JiebaSegmenter.SegMode;
import com.huaban.analysis.jieba.SegToken;

public class Engine {
	public final String AND = "AND", OR = "OR", NOT = "NOT";
	private Search searcher = new Search();			//索引类
	private String lastSearch = "";					//上次搜索
	private String resultHtml = "";					//文件result.html
	JiebaSegmenter segmenter = new JiebaSegmenter();//“结巴”分词
	public Engine() throws IOException {
		//读入result.html文件
		StringBuilder htmlBuilder = new StringBuilder();
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream(
				"html" + File.separator + "result.html"), "utf-8"));
		String buffer = bufferedReader.readLine();
		while (buffer != null) {
			htmlBuilder.append(buffer);
			buffer = bufferedReader.readLine();
		}
		bufferedReader.close();
		resultHtml = htmlBuilder.toString();
	}
	public String search(String keyString) throws Exception {
		return search(keyString, 0);
	}
	public String search(String keyString, int index) throws Exception {
		//对搜索请求字串分词
		List<SegToken> tokens = segmenter.process(keyString, SegMode.SEARCH);
		List<String> keyList = new ArrayList<String>();
		for (SegToken token : tokens) {
			if (token.word.matches("\\s+")) {
				continue;
			}
				keyList.add(token.word);
		}
		
		String[] keyArray = new String[keyList.size()];
		if (keyString.equals(lastSearch) == false) {
			//如果是新的搜索请求，则发起搜索
			getResults((String[])keyList.toArray(keyArray));
			lastSearch = keyString;
		}
		for (int i = 0; i < keyList.size(); i ++) {
			//去掉布尔检索的连接词
			if (keyList.get(i).equalsIgnoreCase(AND)|| keyList.get(i).equalsIgnoreCase(OR)
					|| keyList.get(i).equalsIgnoreCase(NOT)) {
				keyList.remove(i);
				i --;
			}
		}
		keyArray = new String[keyList.size()];
		//生成返回结果的html文件
		return createHtml(keyString, (String[])keyList.toArray(keyArray), index);
	}
	
	private ArrayList<Result> results = new ArrayList<Result>();
	private void getResults(String[] keyArray) {
		results = searcher.search(keyArray);
	}
	
	private String createHtml(String keyString, String[] keyArray, int index) throws Exception {
		StringBuilder htmlBuilder = new StringBuilder(resultHtml);
		
		//生成title并替换到result.html中
		int title = htmlBuilder.indexOf("<!--title-->");
		htmlBuilder.replace(title, title + "<!--title-->".length(), keyString);
		
		//生成content并替换到result.html中
		String resultContent = createContent(keyArray, index);
		int content = htmlBuilder.indexOf("<!--content-->");
		htmlBuilder.replace(content, content + "<!--content-->".length(), resultContent);
		
		//生成bottom并替换到result.html中
		String resultBottom = createBottom(index);
		int bottom = htmlBuilder.indexOf("<!--bottom-->");
		htmlBuilder.replace(bottom, bottom + "<!--bottom-->".length(), resultBottom);
		return htmlBuilder.toString();
	}
	private final int MAX_PER_PAGE = 10, MAX_INDEX = 10;
	private String createContent(String[] keyArray, int index) throws Exception {
		StringBuilder content = new StringBuilder();
		//生成一页的搜索结果
		for (int i = index * 10; i < index * 10 + MAX_PER_PAGE && i < results.size(); i ++) {
			Result r = results.get(i);
			WebPage page = new WebPage(r.index);
			String title = page.getTitle();
			if (title.isEmpty()) { // 找不到title则丢弃这条结果
				results.remove(i);
				i --;
				continue;
			}
			content.append("<div class=\"result\"><h3><a href=\"http://" + r.url + "\">");
			
			content.append(title);
			content.append("</a></h3>" + "<cite>pagerank:" + r.pagerank + " - degreerank"
					+ r.degreerank + "</cite><p class=\"rp\">");
			// getPreview(key): 获得需要预览的信息
			content.append(page.getPreview(keyArray));
			content.append("</p></div>");
		}
		return content.toString();
	}
	private String createBottom(int index) {
		index ++;
		StringBuilder bottom = new StringBuilder();
		//生成底部的html代码：即对应翻页和跳页的交互界面
		if (index > 1) {
			bottom.append("<a class=\"index\" id=\"pre\" href=\"/" + (index - 1) 
					+ ".index\">&lt&lt</a>");
		}
		int beg = index > MAX_INDEX / 3 ? index - MAX_INDEX / 3 : 1;
		for (int i = beg; i <= results.size() / MAX_PER_PAGE && i - beg < MAX_INDEX; i ++) {
			if (i == index) {
				bottom.append("<strong class=\"index\" id=\"cur\">" + index + "</strong>");
			}
			else {
				bottom.append("<a class=\"index\" href=\"/" + i + ".index\">" + i + "</a>");
			}
		}
		if (index < results.size() / MAX_PER_PAGE) {
			bottom.append("<a class=\"index\" id=\"suc\" href=\"/" + (index + 1) 
					+ ".index\">&gt&gt</a>");
		}
		return bottom.toString();
	}
}

