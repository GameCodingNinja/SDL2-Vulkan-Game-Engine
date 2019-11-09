package org.howiesfunware.PachinkoChallenge;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.graphics.Color;

import org.libsdl.app.SDLActivity;

public class PachinkoChallengeActivity extends SDLActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        /*getWindow().setFlags(
            android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN,
            android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // This work only for android 4.4+
        if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.KITKAT)
        {
            getWindow().getDecorView().setSystemUiVisibility(m_systemFlags);

            // Code below is to handle presses of Volume up or Volume down.
            // Without this, after pressing volume buttons, the navigation bar will
            // show up and won't hide
            final View decorView = getWindow().getDecorView();
            decorView.setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener()
                {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility)
                    {
                        if((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0)
                        {
                            decorView.setSystemUiVisibility(m_systemFlags);
                        }
                    }
                });
        }*/
    }
}
