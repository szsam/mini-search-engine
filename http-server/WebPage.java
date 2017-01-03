package http.demo;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class WebPage {
	private String page;		//html文件
	private static final String HTMLFOLDER = "html";
	public WebPage(int index) throws IOException {
		//读取html文件
		BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(
				HTMLFOLDER + File.separator + index + ".html"), "gbk"));
		StringBuilder builder = new StringBuilder();
		String line = reader.readLine();
		while (line != null) {
			builder.append(line);
			line = reader.readLine();
		}
		reader.close();
		page = builder.toString();
	}
	//通过模拟浏览器行为，抓取html页面，此构造方法被弃用
	public WebPage(String url) throws Exception {
		URL readlUrl = new URL("http://" + url);
		URLConnection conn = readlUrl.openConnection();
		conn.setRequestProperty("accept", "*/*");  
        conn.setRequestProperty("connection", "Keep-Alive");  
        conn.setRequestProperty("user-agent",  
                "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");  
        try {
        	conn.connect();
        }
        catch (Exception e) {
        	page = "";
        	return;
        }
        
        Map<String, List<String>> map = conn.getHeaderFields();
        List<String> strings = map.get("Content-Type"); 
        String charset = "utf-8";
        if (strings != null) {
        	for (String string : strings) {
        		if (string.indexOf("charset=") != -1) {
        			charset = string.substring(string.indexOf("charset=") + "charset=".length());
        		}
        }
        }
        BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream(), charset));
        String line = reader.readLine();
        StringBuilder builder = new StringBuilder();
        while (line != null) {
        	builder.append(line + "\n");
        	line = reader.readLine();
        }
        page = builder.toString();
	}
	//获得title
	public String getTitle() {
		String pattern = "<title>(.*)</title>";
		Pattern r = Pattern.compile(pattern, Pattern.DOTALL);
		Matcher m = r.matcher(page);
		if (m.find() == false) {
			return "";
		}
		return m.group(1);
	}
	//获得预览信息
	public String getPreview(String[] keyArray) {
		// 用正则表达式匹配关键词
		StringBuilder keyPattern = new StringBuilder("(" + keyArray[0]);
		for (int i = 1; i < keyArray.length; i ++) {
			keyPattern.append("|").append(keyArray[i]);
		}
		keyPattern.append(")");
		String pattern = ">([^\"\'<>]*?" + keyPattern.toString() + "[^\"\'<>]*?)</(?!script|style)";
		Pattern r = Pattern.compile(pattern, Pattern.DOTALL);
		// 匹配 <body></body>标签内的字符串，尽量匹配到正文
		int body = page.indexOf("<body");
		if (body == -1) {
			body = 0;
		}
		Matcher m = r.matcher(page.substring(body));
		StringBuilder builder = new StringBuilder();
		ArrayList<String> strings = new ArrayList<String>(); 
		while (m.find()) {
			strings.add(m.group(1));
		}
		// 再次匹配全文，以免前面匹配的信息不够
		pattern = ">([^\"\'<>]*?)</(?!script|style)";
		r = Pattern.compile(pattern, Pattern.DOTALL);
		m = r.matcher(page);
		while (m.find()) {
			strings.add(m.group(1));
		}
		
		for (String s : strings) {
			// 替换掉html的转义字符
			s = s.replaceAll("(\\s+|&\\w+;|&#\\d+;)", "");
			if (s.isEmpty()) {
				continue;
			}
			for (int i = 0; i < keyArray.length; i ++) {
				// 给关键字添加<em></em>标签，重点强调
				s = s.replaceAll(keyArray[i], "<em>" + keyArray[i] + "</em>");
			}
			// 用空格隔开
			builder.append(s + "&nbsp;&nbsp;");
		}
		return builder.toString();
	}
}
