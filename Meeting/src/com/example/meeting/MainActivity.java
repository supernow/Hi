package com.example.meeting;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {

	public static EditText policyText;
	public static String fileDirPath = android.os.Environment
			.getExternalStorageDirectory().getAbsolutePath();// 得到外部存储卡的数据库的路径名
	public static String fileName = "record";// 要存储的文件名
	public static Context context;
	Button[] btn = new Button[10];
	CheckBox[] check = new CheckBox[30];
	EditText edit1 = null;
	EditText edit2 = null;
	EditText edit3 = null;
	EditText edit4 = null;
	Button policy;
	Thread myThread = new Thread(new copyFile());

	private Handler myHandler = new Handler() {
		public void handleMessage(Message msg) {
			Toast.makeText(MainActivity.this, "数据已经准备好", Toast.LENGTH_SHORT)
					.show();
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		myThread.start();
		
		btn[2] = (Button) findViewById(R.id.btn_setting3);
		btn[5] = (Button) findViewById(R.id.btn_setting6);
		btn[6] = (Button) findViewById(R.id.btn_setting7);
		context = this;
		policyText = (EditText) findViewById(R.id.policy_edit);
		policy = (Button) findViewById(R.id.policy);

		check[0] = (CheckBox) findViewById(R.id.num11);
		check[1] = (CheckBox) findViewById(R.id.num12);
		check[2] = (CheckBox) findViewById(R.id.num13);
		check[3] = (CheckBox) findViewById(R.id.num14);
		check[4] = (CheckBox) findViewById(R.id.num15);
		check[5] = (CheckBox) findViewById(R.id.num16);
		check[6] = (CheckBox) findViewById(R.id.num21);
		check[7] = (CheckBox) findViewById(R.id.num22);
		check[8] = (CheckBox) findViewById(R.id.num23);
		check[9] = (CheckBox) findViewById(R.id.num24);
		check[10] = (CheckBox) findViewById(R.id.num25);
		check[11] = (CheckBox) findViewById(R.id.num26);
		check[12] = (CheckBox) findViewById(R.id.num31);
		check[13] = (CheckBox) findViewById(R.id.num32);
		check[14] = (CheckBox) findViewById(R.id.num33);
		check[15] = (CheckBox) findViewById(R.id.num34);
		check[16] = (CheckBox) findViewById(R.id.num35);
		check[17] = (CheckBox) findViewById(R.id.num36);
		check[18] = (CheckBox) findViewById(R.id.num41);
		check[19] = (CheckBox) findViewById(R.id.num42);
		check[20] = (CheckBox) findViewById(R.id.num43);
		check[21] = (CheckBox) findViewById(R.id.num44);
		check[22] = (CheckBox) findViewById(R.id.num45);
		check[23] = (CheckBox) findViewById(R.id.num46);
		check[24] = (CheckBox) findViewById(R.id.num51);
		check[25] = (CheckBox) findViewById(R.id.num52);
		check[26] = (CheckBox) findViewById(R.id.num53);
		check[27] = (CheckBox) findViewById(R.id.num54);
		check[28] = (CheckBox) findViewById(R.id.num55);
		check[29] = (CheckBox) findViewById(R.id.num56);

		btn[2].setOnClickListener(onClick.listener);
		btn[5].setOnClickListener(onClick.listener);
		btn[6].setOnClickListener(onClick.listener);
		policy.setOnClickListener(onClick.listener);

		check[0].setOnCheckedChangeListener(onClick.checkListener);
		check[1].setOnCheckedChangeListener(onClick.checkListener);
		check[2].setOnCheckedChangeListener(onClick.checkListener);
		check[3].setOnCheckedChangeListener(onClick.checkListener);
		check[4].setOnCheckedChangeListener(onClick.checkListener);
		check[5].setOnCheckedChangeListener(onClick.checkListener);
		check[6].setOnCheckedChangeListener(onClick.checkListener);
		check[7].setOnCheckedChangeListener(onClick.checkListener);
		check[8].setOnCheckedChangeListener(onClick.checkListener);
		check[9].setOnCheckedChangeListener(onClick.checkListener);
		check[10].setOnCheckedChangeListener(onClick.checkListener);
		check[11].setOnCheckedChangeListener(onClick.checkListener);
		check[12].setOnCheckedChangeListener(onClick.checkListener);
		check[13].setOnCheckedChangeListener(onClick.checkListener);
		check[14].setOnCheckedChangeListener(onClick.checkListener);
		check[15].setOnCheckedChangeListener(onClick.checkListener);
		check[16].setOnCheckedChangeListener(onClick.checkListener);
		check[17].setOnCheckedChangeListener(onClick.checkListener);
		check[18].setOnCheckedChangeListener(onClick.checkListener);
		check[19].setOnCheckedChangeListener(onClick.checkListener);
		check[20].setOnCheckedChangeListener(onClick.checkListener);
		check[21].setOnCheckedChangeListener(onClick.checkListener);
		check[22].setOnCheckedChangeListener(onClick.checkListener);
		check[23].setOnCheckedChangeListener(onClick.checkListener);
		check[24].setOnCheckedChangeListener(onClick.checkListener);
		check[25].setOnCheckedChangeListener(onClick.checkListener);
		check[26].setOnCheckedChangeListener(onClick.checkListener);
		check[27].setOnCheckedChangeListener(onClick.checkListener);
		check[28].setOnCheckedChangeListener(onClick.checkListener);
		check[29].setOnCheckedChangeListener(onClick.checkListener);
	}

	class copyFile implements Runnable {

		public void run() {
			MainActivity point = MainActivity.this;
			for (int i = 0; i < 6; i++) {
				creatFile.create(i, point);
			}
			Message msg = new Message();
			msg.what = 0;
			msg.obj = 0;
			myHandler.sendMessage(msg);
		}

	}

}
