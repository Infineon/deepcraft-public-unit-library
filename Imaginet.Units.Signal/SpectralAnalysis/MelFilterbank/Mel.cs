using System;
using System.Collections.Generic;
using System.Linq;
using Imaginet.Viper.Object;
using Imaginet.Viper.Object.Expressions;

namespace Imaginet.Units.Signal.MelFilterbank;

/// <summary>
/// Mel
/// </summary>
public static class Mel
{
    public static short[] MelFilterPoints(int minFreq, int maxFreq, int numFilters, int inputSize, int sampleRate)
    {
        var alpha = 2595.0;
        var beta = 700.0;

        int fft_size = (inputSize - 1) * 2;
        short freqs_quant(double f) => (short)Math.Floor((fft_size + 1) * f / sampleRate);

        double freq_to_mel(int freq) => alpha * Math.Log10(1.0 + freq / beta);
        double mel_to_freq(double mel) => beta * (Math.Pow(10, mel / alpha) - 1.0);

        //if (!htk)
        //{
        //    throw new NotImplementedException("Only HTK is implemented.");
        //    // See https://github.com/librosa/librosa/blob/e11a7cc360c430001e851855b97dcf3836098a78/librosa/core/convert.py#L822
        //}

        double linspace(double min, double max, int num, int index)
        {
            var rate = index / (num - 1.0);

            return rate * max + (1 - rate) * min;
        }

        var fminMel = freq_to_mel(minFreq);
        var fmaxMel = freq_to_mel(maxFreq);

        short filter_point(int index)
        {
            var mel = linspace(fminMel, fmaxMel, numFilters + 2, index);
            var freq = mel_to_freq(mel);
            return freqs_quant(freq);
        }

        var filters = new short[numFilters + 2];
        for (int i = 0; i < numFilters + 2; i++)
        {
            filters[i] = filter_point(i);
        }

        return filters;
    }

    public static float[] MelFilterCoefsF32(ITensor filterPoints)
    {
        var points = (short[])filterPoints.InitValues;
        int numFilters = points.Length - 2;
        var coef = new List<float>();

        for (int filter = 0; filter < numFilters; filter++)
        {
            int n0 = points[filter];
            int n1 = points[filter + 1];
            int n2 = points[filter + 2];
            int c0 = n1 - n0;
            int c1 = n2 - n1;

            for (int i = 0; i <= c0; i++)
            {
                coef.Add(i / (float)c0);
            }

            for (int i = 1; i <= c1; i++)
            {
                coef.Add(1f - (i / (float)c1));
            }
        }

        return coef.ToArray();
    }

    public static int[] MelFilterCoefsQ31(ITensor filterPoints)
    {
        var f = MelFilterCoefsF32(filterPoints);

       return f.Select(x => QConvert.ConvertToQ31(x, 0)).ToArray();
    }

    public static short[] MelFilterCoefsQ15(ITensor filterPoints)
    {
        var f = MelFilterCoefsF32(filterPoints);

        return f.Select(x => QConvert.ConvertToQ15(x, 0)).ToArray();
    }

}