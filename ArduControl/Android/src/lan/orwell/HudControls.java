package lan.orwell;

import android.graphics.Point;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class HudControls implements OnTouchListener{

	private int _position_x = 20;
	private int _position_y = 400;
	public Point _touchingPoint = new Point(_position_x + 37, _position_y + 37);
	private Boolean _dragging = false;

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		update(event);
		return true;
	}

	private MotionEvent lastEvent;
	public void update(MotionEvent event) {

		if (event == null && lastEvent == null) {
			return;
		} else if (event == null && lastEvent != null) {
			event = lastEvent;
		} else {
			lastEvent = event;
		}

		//drag drop 
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			_dragging = true;
		} else if (event.getAction() == MotionEvent.ACTION_UP) {
			_dragging = false;
		}
		
		if (_dragging && (int)event.getX() > 20 && (int)event.getX() < 20 + 180){
			_touchingPoint.y = (int)event.getY() - 200;
			
			if (_touchingPoint.y < _position_y - 250) {
				_touchingPoint.y = _position_y - 250;
			} 
			if (_touchingPoint.y > _position_y + 37) {
				_touchingPoint.y = _position_y + 37;
			}
		}
	}
}
