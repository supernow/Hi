package com.example.meeting;

public class Meeting {
	static {
		System.loadLibrary("meeting");
		
	}
	public final static native void createConference();
	
	public final static native void joinConference(int number);
	public final static native void dropConference(int number);
	
	public final static native void networkDelay(int number);
	public final static native void newtworkLoss(int number);
	
	public final static native void conferenceOver();
	public final static native void setPolicy(int number);
	
	
	public final static native byte[] getData();
}
