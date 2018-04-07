using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.Audio;

public class PauseMenu : MonoBehaviour
{

    public static bool GameIsPaused = false;
    public static bool InOptionsMenu = false;
    public static bool InCreditsMenu = false;
    public static bool InVolumeMenu = false;
    public static bool InControlsMenu = false;

    public GameObject pauseMenuUI;
    public GameObject menuUI;
    public GameObject creditsUI;
    public GameObject volumeUI;
    public GameObject controlsUI;

	





    // Update is called once per frame
    void Update()
    {   //checks for cases where you are in the main menu 
		//and decided where to go next after your button push
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Debug.Log("ESC pressed");
            if (GameIsPaused == true && InOptionsMenu == true)
            {
                BackToPauseMenu();
            }
            else if (GameIsPaused == true)
            {
                Resume();
            }
            else if (GameIsPaused == true && InCreditsMenu == true)
            {
                ExitCreditsMenu();
            }
            else if (GameIsPaused == true && InVolumeMenu == true)
            {
                ExitVolumeMenu();
            }
            else if (GameIsPaused == true && InControlsMenu == true)
            {
                ExitControlsMenu();
            }
            else if (GameIsPaused == false)
            {
                Paused();
            }
        }

    }

	//what to do for each condition
	//usually it is stop/start time and stop/start audio
	//and open different objects or different scenes depending what you push.
	

	public void ExitCreditsMenu()
    {
        Time.timeScale = 0f;
        pauseMenuUI.SetActive(true);
        creditsUI.SetActive(false);


    }
    public void ExitVolumeMenu()
    {
        Time.timeScale = 0f;
        pauseMenuUI.SetActive(true);
        volumeUI.SetActive(false);


    }
    public void ExitControlsMenu()
    {
        Time.timeScale = 0f;
        pauseMenuUI.SetActive(true);
        controlsUI.SetActive(false);
    }

    public void BackToPauseMenu()
    {
        Time.timeScale = 0f;
        pauseMenuUI.SetActive(true);
        menuUI.SetActive(false);
    }
    public void Resume()
    {
        pauseMenuUI.SetActive(false);
        Time.timeScale = 1f;
        GameIsPaused = false;
		AudioListener.pause = false;
		//AudioListener.pause = false;
	}
    public void Paused()
    {
        pauseMenuUI.SetActive(true);
        Time.timeScale = 0f;
        GameIsPaused = true;
		AudioListener.pause = true;
		//AudioListener.pause = true;
	}
    public void LoadMenu()
    {
        pauseMenuUI.SetActive(false);
        Time.timeScale = 0f;
        GameIsPaused = true;
        menuUI.SetActive(true);
        SceneManager.LoadScene("Menu");
        InOptionsMenu = true;
    }
    public void QuitGame()
    {
        Debug.Log("Quit Game");
        SceneManager.LoadScene(0);
		Time.timeScale = 1f;
		AudioListener.pause = false;
	}



}
