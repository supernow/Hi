package com.example.meeting;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import android.content.Context;

public class creatFile {
	public static void create(int i, MainActivity point) {
		String filePath = MainActivity.fileDirPath + "/"
				+ MainActivity.fileName + i + ".pcm";// 文件路径
		try {
			File dir = new File(filePath);// 目录路径
			if (dir.exists()) {// 如果不存在，则创建路径名
				dir.delete();
			}
			// 目录存在，则将apk中raw中的需要的文档复制到该目录下
			File file = new File(filePath);
			if (!file.exists()) {// 文件不存在
				System.out.println("要打开的文件不存在");
				Context context = point.getApplicationContext();
				InputStream ins = context.getAssets().open(
						"record" + i + ".pcm");
				System.out.println("开始读入");
				FileOutputStream fos = new FileOutputStream(file);
				System.out.println("开始写出");
				byte[] buffer = new byte[8192];
				int count = 0;// 循环写出
				while ((count = ins.read(buffer)) > 0) {
					fos.write(buffer, 0, count);
				}
				System.out.println("已经创建该文件");
				fos.close();// 关闭流
				ins.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
