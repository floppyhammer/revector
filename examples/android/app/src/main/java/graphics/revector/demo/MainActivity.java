package graphics.revector.demo;

import com.google.androidgamesdk.GameActivity;

import android.os.Bundle;

public class MainActivity extends GameActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    static {
        System.loadLibrary("revector_android_demo");
    }
}
