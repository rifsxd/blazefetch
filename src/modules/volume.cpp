#include "helper.cpp"

// Function to get the current audio volume
std::string getAudioVolumeInfo() {
    long min, max, volume;
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_id_t *sid;

    const char *card = "default";
    const char *selem_name = "Master";

    // Open mixer
    if (snd_mixer_open(&handle, 0) < 0) {
        return "Error: Unable to open mixer";
    }

    // Attach the mixer handle to the given card
    if (snd_mixer_attach(handle, card) < 0) {
        snd_mixer_close(handle);
        return "Error: Unable to attach mixer to card";
    }

    // Register mixer elements
    if (snd_mixer_selem_register(handle, NULL, NULL) < 0) {
        snd_mixer_close(handle);
        return "Error: Unable to register mixer elements";
    }

    // Load the mixer elements
    if (snd_mixer_load(handle) < 0) {
        snd_mixer_close(handle);
        return "Error: Unable to load mixer elements";
    }

    snd_mixer_selem_id_alloca(&sid);

    // Set the simple element name
    snd_mixer_selem_id_set_name(sid, selem_name);

    // Get the mixer element
    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        snd_mixer_close(handle);
        return "Error: Unable to find simple control";
    }

    // Get the volume range
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

    // Get the current volume
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO, &volume);

    // Close the mixer
    snd_mixer_close(handle);

    std::string volumeString;
    if (volume == 0)
        volumeString = std::string(VOLUME_MUTED) + + "\033[0m" + " " + "Muted!";
    else {
        // Calculate percentage
        int percent = ((volume - min) * 100) / (max - min);
        
        if (percent < 50)
            volumeString = std::string(VOLUME_LOW) + "\033[0m";
        else
            volumeString = std::string(VOLUME_HIGH) + "\033[0m";

        volumeString += " " + std::to_string(percent) + "%";
    }

    return "\033[33m" + volumeString;
}