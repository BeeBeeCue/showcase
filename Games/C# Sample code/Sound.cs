using UnityEngine.Audio;
using UnityEngine;

[System.Serializable]
public class Sound {

	public string name;



	public AudioClip clip;
    //Set volume for clip
	[Range(0f, 1f)]
	public float volume = .75f;
    //The volume can vary if effect is wanted
	[Range(0f, 1f)]
	public float volumeVariance = .1f;
    //Set pitch for clip
	[Range(.1f, 3f)]
	public float pitch = 1f;
    //The pitch can vary if effect is wanted
	[Range(0f, 1f)]
	public float pitchVariance = .1f;

    //default is not to loop
	public bool loop = false;

    //I can route the signal to a different mixer group
	public AudioMixerGroup mixerGroup;

	[HideInInspector]
	public AudioSource source;

}
