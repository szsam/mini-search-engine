package demo.Extracter;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.huaban.analysis.jieba.JiebaSegmenter;
import com.huaban.analysis.jieba.JiebaSegmenter.SegMode;
import com.huaban.analysis.jieba.SegToken;

public class KeysExtracter {
	private final String SRC_FOLDER = "html", DST_FOLDER = "keys",
			SUFFIX = ".html", DST_FILE = "result.txt";
	
	private void extract() throws IOException {
		int i = 0;
		String dst = DST_FOLDER + File.separator + DST_FILE;
		OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(dst), "gbk");
		BufferedReader reader;
		while(true) {
			// 依次读入html文件，直到读取失败
			String file = SRC_FOLDER + File.separator + i + SUFFIX;
			try {
				reader = new BufferedReader(new InputStreamReader(new FileInputStream(file), "gbk"));
			} catch (FileNotFoundException e) {
				System.out.println("stop at " + i);
				break;
			}
			StringBuilder page = new StringBuilder();
			String tmp = reader.readLine();
			while (tmp != null) {
				page.append(tmp);
				tmp = reader.readLine();
			}
			reader.close();
			// getKeys: 获得分词结果
			String result = getKeys(page.toString());
			writer.write(i + "\n");
			writer.write(result + "\n");
			i ++;
			if (i % 100 == 0) {
				System.out.println("at " + i);
			}
		}
		writer.close();
	}
	//分词方法
	private String getKeys(String string) {
		StringBuilder keys = new StringBuilder(), content = new StringBuilder();
		//正则匹配出不是<script>标签也不是<style>标签内的内容
		String pattern = ">([^\"\'<>]*?)</(?!script|style)"; 
		Pattern r = Pattern.compile(pattern, Pattern.DOTALL);
		Matcher m = r.matcher(string);
		while (m.find()) {
			//替换掉html的转义字符
			content.append(" ").append(m.group(1).replaceAll("(&\\w+;|&#\\d+;)", " "));
		}
		JiebaSegmenter segmenter = new JiebaSegmenter();
		//进行分词
		List<SegToken> tokens = segmenter.process(content.toString(), SegMode.INDEX);
		for (SegToken token : tokens) {
			String word = token.word;
			//匹配出中文、英文或数字的字串
			if (word.matches("[\\u3400-\\u9FA5a-zA-Z0-9]+")) {
				keys.append(word).append(" ");
			}
		}
		//删除最后一个空格
		if (keys.length() > 0) {
			keys.delete(keys.length() - 1, keys.length());
		}
		return keys.toString();
	}

	public static void main(String[] args) throws IOException {
		KeysExtracter extracter = new KeysExtracter();
		extracter.extract();
	}
}
