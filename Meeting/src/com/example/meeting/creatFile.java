package com.example.meeting;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import android.content.Context;

public class creatFile {
	public static void create(int i, MainActivity point) {
		String filePath = MainActivity.fileDirPath + "/"
				+ MainActivity.fileName + i + ".pcm";// �ļ�·��
		try {
			File dir = new File(filePath);// Ŀ¼·��
			if (dir.exists()) {// ��������ڣ��򴴽�·����
				dir.delete();
			}
			// Ŀ¼���ڣ���apk��raw�е���Ҫ���ĵ����Ƶ���Ŀ¼��
			File file = new File(filePath);
			if (!file.exists()) {// �ļ�������
				System.out.println("Ҫ�򿪵��ļ�������");
				Context context = point.getApplicationContext();
				InputStream ins = context.getAssets().open(
						"record" + i + ".pcm");
				System.out.println("��ʼ����");
				FileOutputStream fos = new FileOutputStream(file);
				System.out.println("��ʼд��");
				byte[] buffer = new byte[8192];
				int count = 0;// ѭ��д��
				while ((count = ins.read(buffer)) > 0) {
					fos.write(buffer, 0, count);
				}
				System.out.println("�Ѿ��������ļ�");
				fos.close();// �ر���
				ins.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
