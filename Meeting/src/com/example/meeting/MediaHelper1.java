package com.example.meeting;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;

abstract class MHRunnable1 implements Runnable
{
	public MHRunnable1(Object o)
	{
		obj = o;
	}

	@Override
	public void run()
	{
		run(obj);
	}

	public abstract void run(Object o);

	private Object obj;
}

class RecordParam1
{
	public RecordParam1(int bits, int rate, int nchannels, String filepath, int timeout)
	{
		this.bits = bits;
		this.rate = rate;
		this.nchannels = nchannels;
		this.filepath = filepath;
		this.timeout = timeout;
	}

	int bits;

	int rate;

	int nchannels;

	String filepath;

	int timeout;
}

class PlayParam1
{
	public PlayParam1(int bits, int rate, int nchannels, String filepath)
	{
		this.bits = bits;
		this.rate = rate;
		this.nchannels = nchannels;
		this.filepath = filepath;
	}

	int bits;

	int rate;

	int nchannels;

	String filepath;
}

class DeviceRunTime1
{
	public long begRecordTick = 0;

	public long begPlayTick = 0;

	public long recordBufLen = 0;

	public long playBufLen = 0;
}

@SuppressLint({ "NewApi", "NewApi" })
@TargetApi(16)
public class MediaHelper1
{
	private static MediaHelper1 inst = null;

	private static DeviceRunTime drt = new DeviceRunTime();

	private MediaHelper1()
	{
	}

	public static MediaHelper1 instance()
	{
		if (inst == null)
			inst = new MediaHelper1();
		return inst;
	}

	DeviceRunTime getDeviceRunTime()
	{
		return drt;
	}

	public void setCallback(IMediaHelperCb mscb)
	{
		inst.mscb = mscb;
	}

	public int playFile(String filepath,int RATE_RATE)
	{
		new Thread(new MHRunnable(new PlayParam(16, RATE_RATE, 1, filepath))
		{
			public void run(Object o)
			{
				System.out.println("start play.");

				PlayParam param = (PlayParam) o;
				int frameLen = param.rate*20/1000*param.bits/8;//320; // 20ms

				AudioTrack audioTrack = null;
				DataInputStream dis = null;
				try
				{
					dis = new DataInputStream(new BufferedInputStream(
							new FileInputStream(new File(param.filepath))));

					int bufsize = android.media.AudioTrack.getMinBufferSize(
							param.rate, AudioFormat.CHANNEL_OUT_MONO,
							(param.bits == 16) ? AudioFormat.ENCODING_PCM_16BIT
									: AudioFormat.ENCODING_PCM_8BIT);

					audioTrack = new AudioTrack(AudioManager.STREAM_VOICE_CALL,
							param.rate, AudioFormat.CHANNEL_OUT_MONO,
							(param.bits == 16) ? AudioFormat.ENCODING_PCM_16BIT
									: AudioFormat.ENCODING_PCM_8BIT, bufsize,
							AudioTrack.MODE_STREAM);
					
					audioTrack.play();

					if (audioTrack == null || dis == null)
						return;

					isPlaying = true;
					byte[] buffer = new byte[frameLen];
					if (mscb != null)
						mscb.notifyEvt(IMediaHelperCb.playfile_beg, param.filepath);
					while (isPlaying)
					{
						int length = dis.read(buffer, 0, frameLen);
						if (length <= 0)
						{
							break;
						}

						audioTrack.write(buffer, 0, frameLen);
					}

					if (mscb != null)
						mscb.notifyEvt(IMediaHelperCb.playfile_end, param.filepath);
				}
				catch (Throwable e)
				{
					e.printStackTrace();
				}
				finally
				{
					if (audioTrack != null)
					{
						audioTrack.stop();
						audioTrack.release();
					}
					try
					{
						if (dis != null)
							dis.close();
					}
					catch (IOException e)
					{
						e.printStackTrace();
					}
				}
			}
		}).start();

		return 0;
	}

	public void stopPlay()
	{
		isPlaying = false;
	}

	public int recordFile(String filepath, int timeout, int RATE_RATE)
	{
		new Thread(new MHRunnable1(new RecordParam1(16, RATE_RATE, 1, filepath, timeout))
		{
			public void run(Object o)
			{
				RecordParam1 param = (RecordParam1) o;

				System.out.println("start record[" + param.filepath + "].");
				
				int frameLen = param.rate*20/1000*param.bits/8;//320; // 20ms
				int framenum = param.timeout * 1000 / 20; // 20ms

				AudioRecord audioRecord = null;
				DataOutputStream dos = null;

				try
				{
					dos = new DataOutputStream(new BufferedOutputStream(
							new FileOutputStream(new File(param.filepath))));

					int bufferSize = AudioRecord.getMinBufferSize(param.rate,
							AudioFormat.CHANNEL_IN_MONO,
							(param.bits == 16) ? AudioFormat.ENCODING_PCM_16BIT
									: AudioFormat.ENCODING_PCM_8BIT);
					System.out.println("record buffer size:" + bufferSize);

					audioRecord = new AudioRecord(
							MediaRecorder.AudioSource.MIC, param.rate,
							AudioFormat.CHANNEL_IN_MONO,
							(param.bits == 16) ? AudioFormat.ENCODING_PCM_16BIT
									: AudioFormat.ENCODING_PCM_8BIT, bufferSize);

					if (audioRecord == null || dos == null)
						return;

					if (mscb != null)
						mscb.notifyEvt(IMediaHelperCb.recordfile_beg, param.filepath);
					byte[] buffer = new byte[frameLen];
					drt.begRecordTick = System.currentTimeMillis();
					audioRecord.startRecording();
					drt.recordBufLen = ((System.currentTimeMillis() - drt.begRecordTick) / 20 - 1) * 20;

					isRecording = true;
					while (isRecording && (framenum-- >= 0))
					{
						audioRecord.read(buffer, 0, frameLen);
						dos.write(buffer);
					}

					if (isRecording)
					{
						System.out.println("record timeout.");
					}

					if (mscb != null)
						mscb.notifyEvt(IMediaHelperCb.recordfile_end, param.filepath);
				}
				catch (Throwable e)
				{
					e.printStackTrace();
				}
				finally
				{
					if (audioRecord != null)
					{
						audioRecord.stop();
						audioRecord.release();
					}
					try
					{
						if (dos != null)
							dos.close();
					}
					catch (IOException e)
					{
						e.printStackTrace();
					}
				}
			}
		}).start();
		return 0;
	}
	
	public void stopRecord()
	{
		isRecording = false;
	}
	
	private IMediaHelperCb mscb = null;

	private boolean isRecording = false;

	private boolean isPlaying = false;
	
}
