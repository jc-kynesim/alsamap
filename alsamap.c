#include <stdio.h>

#include <alsa/asoundlib.h>
#include <alsa/version.h>

#define BLEN 1024

#define XLAT_LEN 64
struct {
    const char * sname;
    const char * lname;
} pos_xlat[XLAT_LEN] = {
    [SND_CHMAP_UNKNOWN] = {"UNKNOWN", "unspecified"},
    [SND_CHMAP_NA] = {"NA", "N/A, silent"},
    [SND_CHMAP_MONO] = {"MONO", "mono stream"},
    [SND_CHMAP_FL] = {"FL", "front left"},
    [SND_CHMAP_FR] = {"FR", "front right"},
    [SND_CHMAP_RL] = {"RL", "rear left"},
    [SND_CHMAP_RR] = {"RR", "rear right"},
    [SND_CHMAP_FC] = {"FC", "front center"},
    [SND_CHMAP_LFE] = {"LFE", "LFE"},
    [SND_CHMAP_SL] = {"SL", "side left"},
    [SND_CHMAP_SR] = {"SR", "side right"},
    [SND_CHMAP_RC] = {"RC", "rear center"},
    [SND_CHMAP_FLC] = {"FLC", "front left center"},
    [SND_CHMAP_FRC] = {"FRC", "front right center"},
    [SND_CHMAP_RLC] = {"RLC", "rear left center"},
    [SND_CHMAP_RRC] = {"RRC", "rear right center"},
    [SND_CHMAP_FLW] = {"FLW", "front left wide"},
    [SND_CHMAP_FRW] = {"FRW", "front right wide"},
    [SND_CHMAP_FLH] = {"FLH", "front left high"},
    [SND_CHMAP_FCH] = {"FCH", "front center high"},
    [SND_CHMAP_FRH] = {"FRH", "front right high"},
    [SND_CHMAP_TC] = {"TC", "top center"},
    [SND_CHMAP_TFL] = {"TFL", "top front left"},
    [SND_CHMAP_TFR] = {"TFR", "top front right"},
    [SND_CHMAP_TFC] = {"TFC", "top front center"},
    [SND_CHMAP_TRL] = {"TRL", "top rear left"},
    [SND_CHMAP_TRR] = {"TRR", "top rear right"},
    [SND_CHMAP_TRC] = {"TRC", "top rear center"},
    [SND_CHMAP_TFLC] = {"TFLC", "top front left center"},
    [SND_CHMAP_TFRC] = {"TFRC", "top front right center"},
    [SND_CHMAP_TSL] = {"TSL", "top side left"},
    [SND_CHMAP_TSR] = {"TSR", "top side right"},
    [SND_CHMAP_LLFE] = {"LLFE", "left LFE"},
    [SND_CHMAP_RLFE] = {"RLFE", "right LFE"},
    [SND_CHMAP_BC] = {"BC", "bottom center"},
    [SND_CHMAP_BLC] = {"BLC", "bottom left center"},
    [SND_CHMAP_BRC] = {"BRC", "bottom right center"},
};

int main(int argc, char*argv[])
{
    int rv;
    const char * device;
    snd_pcm_t *pcm;
    int i;

    if (argc != 2) {
        printf("Usage: alsamap <device>\n");
        exit(1);
    }

    device = argv[1];

    if ((rv = snd_pcm_open(&pcm, device, SND_PCM_STREAM_PLAYBACK, SND_PCM_NO_AUTO_RESAMPLE)) != 0) {
        printf("Failed to open device '%s': rv=%d\n", device, rv);
        exit(1);
    }

    snd_pcm_chmap_query_t **maps = snd_pcm_query_chmaps(pcm);
    if (maps == NULL) {
        printf("Failed to get maps\n");
        exit(1);
    }

    for (i = 0; maps[i] != NULL; ++i) {
        const snd_pcm_chmap_query_t * const m = maps[i];
        char buf[BLEN];
        size_t n = 0;

        n += snprintf(buf + n, BLEN - n, "[%2d] ", i);

        switch (m->type)
        {
            case SND_CHMAP_TYPE_FIXED:
                n += snprintf(buf + n, BLEN - n, "Fixed:");
                break;
            case SND_CHMAP_TYPE_PAIRED:
                n += snprintf(buf + n, BLEN - n, "Paired:");
                break;
            case SND_CHMAP_TYPE_VAR:
                n += snprintf(buf + n, BLEN - n, "Var:");
                break;
            default:
                n += snprintf(buf + n, BLEN - n, "?Type %d?:", m->type);
                break;
        }

        n += snprintf(buf + n, BLEN - n, " Chans=%d:", m->map.channels);

        for (unsigned int j = 0; j != m->map.channels; ++j) {
            const unsigned int pos = m->map.pos[j];

            if (pos >= XLAT_LEN || pos_xlat[pos].sname == NULL) {
                n += snprintf(buf + n, BLEN - n, " ?%d?", pos);
            }
            else {
                n += snprintf(buf + n, BLEN - n, " %s(%d)", pos_xlat[pos].sname, pos);
            }
        }

        puts(buf);
    }

    snd_pcm_close (pcm);
    return 0;
}

