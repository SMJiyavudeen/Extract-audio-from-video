#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>

int main(int argc, char *argv[])
{
    // Initialize libavformat and register all the muxers, demuxers and protocols.
    av_register_all();

    // Open the input video file.
    AVFormatContext *input_format_ctx = NULL;
    if (avformat_open_input(&input_format_ctx, "input.mp4", NULL, NULL) != 0) {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    // Find the audio stream in the input file.
    int audio_stream_index = -1;
    for (int i = 0; i < input_format_ctx->nb_streams; i++) {
        if (input_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }
    if (audio_stream_index == -1) {
        fprintf(stderr, "Input file does not have an audio stream\n");
        return 1;
    }

    // Find the decoder for the audio stream.
    AVCodec *audio_decoder = avcodec_find_decoder(input_format_ctx->streams[audio_stream_index]->codecpar->codec_id);
    if (audio_decoder == NULL) {
        fprintf(stderr, "Unsupported audio codec\n");
        return 1;
    }

    // Allocate a codec context for the decoder.
    AVCodecContext *audio_decoder_ctx = avcodec_alloc_context3(audio_decoder);
    if (audio_decoder_ctx == NULL) {
        fprintf(stderr, "Failed to allocate codec context\n");
        return 1;
    }

    // Initialize the codec context with the information from the input audio stream.
    if (avcodec_parameters_to_context(audio_decoder_ctx, input_format_ctx->streams[audio_stream_index]->codecpar) < 0) {
        fprintf(stderr, "Failed to copy codec context\n");
        return 1;
    }

    // Open the decoder.
    if (avcodec_open2(audio_decoder_ctx, audio_decoder, NULL) != 0) {
        fprintf(stderr, "Failed to open decoder\n");
        return 1;
    }

    // Allocate a frame for holding the decoded audio samples.
    AVFrame *decoded_frame = av_frame_alloc();
    if (decoded_frame == NULL) {
        fprintf(stderr, "Failed to allocate frame\n");
        return

