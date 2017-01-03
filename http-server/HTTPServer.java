package http.demo;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URLDecoder;

public class HTTPServer {
	private final int PORT;			//端口号
	private String last = "";		//上次搜索记录
	private String indexHtml = "";	//文件 index.html
	public HTTPServer(int port) throws IOException {
		PORT = port;
		//读入 index.html 文件
		BufferedReader bufferedReader = new BufferedReader(new FileReader("html" + File.separator + "index.html"));
		String buffer = bufferedReader.readLine();
		StringBuilder builder = new StringBuilder();
		while (buffer != null) {
			builder.append(buffer);
			buffer = bufferedReader.readLine();
		}
		bufferedReader.close();
		indexHtml = builder.toString();
	}
	public void start() throws Exception {
		ServerSocket serverSocket = new ServerSocket(PORT);
		//搜索引擎类
		Engine engine = new Engine();
		System.out.println("HttpServer start...");
		
		while (true) {
			//接收http请求并分析
			Socket socket = serverSocket.accept();
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			String header = bufferedReader.readLine();
			System.out.println(header);
			if (header == null) {
				continue;
			}
			String path = header.split(" ")[1];
			String html, key = "";
			//main page or searching page
			int index = 0;
			//搜索请求
			if (path.indexOf("/?search=") == 0) {
				key = URLDecoder.decode(path.substring("/?search=".length()), "utf-8");
				last = key;
			}
			//翻页请求
			else if (path.matches("/\\d+\\.index")) {
				index = Integer.parseInt(path.substring(1,path.indexOf(".")));
			}
			//空白请求
			if (key.replaceAll("\\s+", "").isEmpty() == false) {
				html = engine.search(key);
			}
			//返回对应翻页的搜索结果
			else if (index != 0) {
				html = engine.search(last, index - 1); // index begin from 0
			}
			else {
				html = indexHtml;
			}
			//生成返回结果的http头部
			OutputStreamWriter writer = new OutputStreamWriter(socket.getOutputStream(), "utf-8");
			writer.write("HTTP/1.1 200 OK\n");
			writer.write("Content-type:text/html;charset=UTF-8\n");
			writer.write("\n");
			writer.write(html);
			
			writer.flush();
			socket.close();
		}
	}
	public static void main(String[] args) throws IOException{
		//启动服务器
		HTTPServer server = new HTTPServer(8080);
		while (true) {
			try {
				server.start();
			}
			catch (Exception e) {
				e.printStackTrace();
				System.out.println("HttpServer crash!...restart now");
			}
		}
	}
}
