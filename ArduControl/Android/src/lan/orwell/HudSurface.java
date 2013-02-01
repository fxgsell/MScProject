package lan.orwell;

import android.content.Context;
import android.graphics.Canvas;
import android.os.Handler;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class HudSurface extends SurfaceView implements SurfaceHolder.Callback {

	private Context _context;
	private HudThread _thread;
	private HudControls _controls;
	private int _position_x = 20;
	private int _position_y = 400;
	private HudJoystick _joystick;

	public HudSurface(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		_context = context;
		init();
	}

	private void init(){
		//initialize our screen holder
		SurfaceHolder holder = getHolder();
		holder.addCallback( this);

		//initialize our game engine

		//initialize our Thread class. A call will be made to start it later
		_thread = new HudThread(holder, _context, new Handler(),this);
		setFocusable(true);


		_joystick = new HudJoystick(getContext().getResources());
		_controls = new HudControls();
		setOnTouchListener(_controls);
	}


	public void doDraw(Canvas canvas){

		//update the pointer
		_controls.update(null);
		
		//draw the joystick background
		canvas.drawBitmap(_joystick.get_joystickBg(), _position_x, _position_y, null);

		//draw the dragable joystick
		canvas.drawBitmap(_joystick.get_joystick(), _controls._touchingPoint.x, _controls._touchingPoint.y, null);

	}



	//these methods are overridden from the SurfaceView super class. They are automatically called 
	//when a SurfaceView is created, resumed or suspended.
	@Override 
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {}
	private boolean retry;
	@Override 
	public void surfaceDestroyed(SurfaceHolder arg0) {
		retry = true;
		//code to end gameloop
		_thread.state = HudThread.STOPED;
		while (retry) {
			try {
				//code to kill Thread
				_thread.join();
				retry = false;
			} catch (InterruptedException e) {
			}
		}

	}

	@Override 
	public void surfaceCreated(SurfaceHolder arg0) {
		if(_thread.state==HudThread.PAUSED){
			//When game is opened again in the Android OS
			_thread = new HudThread(getHolder(), _context, new Handler(),this);
			_thread.start();
		}else{
			//creating the game Thread for the first time
			_thread.start();
		}
	}

	public void Update() {
		// TODO Auto-generated method stub

	}

}
