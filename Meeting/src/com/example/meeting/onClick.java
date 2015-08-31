package com.example.meeting;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;

public class onClick {

	public static OnClickListener listener = new OnClickListener() {
		@Override
		public void onClick(View v) {

			int ID = v.getId();
			switch (ID) {
			case R.id.btn_setting3:
				Meeting.createConference();
				Meeting.joinConference(1);
				Meeting.joinConference(2);
				Meeting.joinConference(3);
				Meeting.joinConference(4);
				Meeting.joinConference(5);
				Meeting.joinConference(6);
				break;

			case R.id.btn_setting6:
				Meeting.conferenceOver();
				MediaHelper.instance().stopPlay();
				break;

			case R.id.btn_setting7:
				MediaHelper.instance().playFile(16, 8000, 1,
						MainActivity.fileDirPath + "/fileOut.pcm");
				break;

			case R.id.policy:
				String text = MainActivity.policyText.getText().toString();
				if (!text.equals("")) {
					Meeting.setPolicy(Integer.parseInt(text));
				}
				break;

			default:
				break;
			}
		}

	};

	public static OnCheckedChangeListener checkListener = new CompoundButton.OnCheckedChangeListener() {

		@Override
		public void onCheckedChanged(CompoundButton buttonView,
				boolean isChecked) {
			int ID = buttonView.getId();
			Integer i = 0;
			switch (ID) {
			case R.id.num11:
				if (isChecked)
					Meeting.joinConference(1);
				break;
			case R.id.num12:
				if (isChecked)
					Meeting.joinConference(2);
				break;
			case R.id.num13:
				if (isChecked)
					Meeting.joinConference(3);
				break;
			case R.id.num14:
				if (isChecked)
					Meeting.joinConference(4);
				break;
			case R.id.num15:
				if (isChecked)
					Meeting.joinConference(5);
				break;
			case R.id.num16:
				if (isChecked)
					Meeting.joinConference(6);
				break;
			case R.id.num21:
				if (isChecked)
					Meeting.dropConference(1);
				break;
			case R.id.num22:
				if (isChecked)
					Meeting.dropConference(2);
				break;
			case R.id.num23:
				if (isChecked)
					Meeting.dropConference(3);
				break;
			case R.id.num24:
				if (isChecked)
					Meeting.dropConference(4);
				break;
			case R.id.num25:
				if (isChecked)
					Meeting.dropConference(5);
				break;
			case R.id.num26:
				if (isChecked)
					Meeting.dropConference(6);
				break;

			case R.id.num31:
				if (isChecked)
					Meeting.newtworkLoss(1);
				break;
			case R.id.num32:
				if (isChecked)
					Meeting.newtworkLoss(2);
				break;
			case R.id.num33:
				if (isChecked)
					Meeting.newtworkLoss(3);
				break;
			case R.id.num34:
				if (isChecked)
					Meeting.newtworkLoss(4);
				break;
			case R.id.num35:
				if (isChecked)
					Meeting.newtworkLoss(5);
				break;
			case R.id.num36:
				if (isChecked)
					Meeting.newtworkLoss(6);
				break;

			case R.id.num41:
				if (isChecked)
					Meeting.networkDelay(1);
				break;
			case R.id.num42:
				if (isChecked)
					Meeting.networkDelay(2);
				break;
			case R.id.num43:
				if (isChecked)
					Meeting.networkDelay(3);
				break;
			case R.id.num44:
				if (isChecked)
					Meeting.networkDelay(4);
				break;
			case R.id.num45:
				if (isChecked)
					Meeting.networkDelay(5);
				break;
			case R.id.num46:
				if (isChecked)
					Meeting.networkDelay(6);
				break;

			case R.id.num51:
				i = 1;
				((MyApplication) MainActivity.context.getApplicationContext())
						.setMyPublicData(i);
				if (isChecked)
					// MediaHelper1.instance().recordFile("/mnt/sdcard/record1.pcm",
					// 10, 8000);
					new AlertDialog.Builder(MainActivity.context)
							.setTitle("单独录音")
							.setMessage("确定吗？")
							.setPositiveButton("开始录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											Integer d = ((MyApplication) MainActivity.context
													.getApplicationContext())
													.getMyPublicData();
											MediaHelper1.instance().recordFile(
													"/mnt/sdcard/record"
															+ (int) d + ".pcm",
													10000000, 8000);
										}
									})
							.setNegativeButton("结束录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											MediaHelper1.instance()
													.stopRecord();
										}
									}).show();
				else
					MediaHelper1.instance().stopRecord();
				break;

			case R.id.num52:
				i = 2;
				((MyApplication) MainActivity.context.getApplicationContext())
						.setMyPublicData(i);
				if (isChecked)
					// MediaHelper1.instance().recordFile("/mnt/sdcard/record1.pcm",
					// 10, 8000);
					new AlertDialog.Builder(MainActivity.context)
							.setTitle("单独录音")
							.setMessage("确定吗？")
							.setPositiveButton("开始录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											Integer d = ((MyApplication) MainActivity.context
													.getApplicationContext())
													.getMyPublicData();
											MediaHelper1.instance().recordFile(
													"/mnt/sdcard/record"
															+ (int) d + ".pcm",
													10000000, 8000);
										}
									})
							.setNegativeButton("结束录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											MediaHelper1.instance()
													.stopRecord();
										}
									}).show();
				else
					MediaHelper1.instance().stopRecord();
				break;

			case R.id.num53:
				i = 3;
				((MyApplication) MainActivity.context.getApplicationContext())
						.setMyPublicData(i);
				if (isChecked)
					// MediaHelper1.instance().recordFile("/mnt/sdcard/record1.pcm",
					// 10, 8000);
					new AlertDialog.Builder(MainActivity.context)
							.setTitle("单独录音")
							.setMessage("确定吗？")
							.setPositiveButton("开始录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											Integer d = ((MyApplication) MainActivity.context
													.getApplicationContext())
													.getMyPublicData();
											MediaHelper1.instance().recordFile(
													"/mnt/sdcard/record"
															+ (int) d + ".pcm",
													10000000, 8000);
										}
									})
							.setNegativeButton("结束录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											MediaHelper1.instance()
													.stopRecord();
										}
									}).show();
				else
					MediaHelper1.instance().stopRecord();
				break;

			case R.id.num54:
				i = 4;
				((MyApplication) MainActivity.context.getApplicationContext())
						.setMyPublicData(i);
				if (isChecked)
					// MediaHelper1.instance().recordFile("/mnt/sdcard/record1.pcm",
					// 10, 8000);
					new AlertDialog.Builder(MainActivity.context)
							.setTitle("单独录音")
							.setMessage("确定吗？")
							.setPositiveButton("开始录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											Integer d = ((MyApplication) MainActivity.context
													.getApplicationContext())
													.getMyPublicData();
											MediaHelper1.instance().recordFile(
													"/mnt/sdcard/record"
															+ (int) d + ".pcm",
													10000000, 8000);
										}
									})
							.setNegativeButton("结束录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											MediaHelper1.instance()
													.stopRecord();
										}
									}).show();
				else
					MediaHelper1.instance().stopRecord();
				break;

			case R.id.num55:
				i = 5;
				((MyApplication) MainActivity.context.getApplicationContext())
						.setMyPublicData(i);
				if (isChecked)
					// MediaHelper1.instance().recordFile("/mnt/sdcard/record1.pcm",
					// 10, 8000);
					new AlertDialog.Builder(MainActivity.context)
							.setTitle("单独录音")
							.setMessage("确定吗？")
							.setPositiveButton("开始录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											Integer d = ((MyApplication) MainActivity.context
													.getApplicationContext())
													.getMyPublicData();
											MediaHelper1.instance().recordFile(
													"/mnt/sdcard/record"
															+ (int) d + ".pcm",
													10000000, 8000);
										}
									})
							.setNegativeButton("结束录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											MediaHelper1.instance()
													.stopRecord();
										}
									}).show();
				else
					MediaHelper1.instance().stopRecord();
				break;

			case R.id.num56:
				i = 6;
				((MyApplication) MainActivity.context.getApplicationContext())
						.setMyPublicData(i);
				if (isChecked)
					// MediaHelper1.instance().recordFile("/mnt/sdcard/record1.pcm",
					// 10, 8000);
					new AlertDialog.Builder(MainActivity.context)
							.setTitle("单独录音")
							.setMessage("确定吗？")
							.setPositiveButton("开始录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											Integer d = ((MyApplication) MainActivity.context
													.getApplicationContext())
													.getMyPublicData();
											MediaHelper1.instance().recordFile(
													"/mnt/sdcard/record"
															+ (int) d + ".pcm",
													10000000, 8000);
										}
									})
							.setNegativeButton("结束录音",
									new DialogInterface.OnClickListener() {

										@Override
										public void onClick(
												DialogInterface dialog,
												int which) {
											MediaHelper1.instance()
													.stopRecord();
										}
									}).show();
				else
					MediaHelper1.instance().stopRecord();
				break;
			default:
				break;
			}
		}

	};
}
