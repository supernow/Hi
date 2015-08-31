package com.example.meeting;

import java.lang.ref.WeakReference;

import android.app.Application;

//MyApplication.java
public class MyApplication extends Application{
	public WeakReference<Integer> d;
	
	public void setMyPublicData(Integer _d){
		d = null;
		d = new WeakReference<Integer>(_d);
	}
	
	public Integer getMyPublicData(){
		return d.get();
	}
}