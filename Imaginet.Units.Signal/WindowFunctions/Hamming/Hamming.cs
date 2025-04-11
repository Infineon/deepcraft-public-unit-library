/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* File created: 2021-08-12 Albert Seward (albert.seward@imagimob.com), Johan Malm (johan.malm@imagimob.com)
*/

using System;
using System.Linq;
using Imaginet.Viper.Object;

namespace Imaginet.Units.Signal.Hamming;

public static class Hamming
{
    /// <summary>
    /// 
    /// </summary>
    /// <param name="size"></param>
    /// <param name="sym"> When True, generates a symmetric window, for use in filter design. 
    /// When False, generates a periodic window, for use in spectral analysis.</param>
    /// <returns></returns>
    public static float[] HammingTableF32(int size, bool sym)
    {
        var data = new float[size];
        var a0 = 0.54; //0.53836;
        var a1 = 0.46; //0.46164;

        for (int i = 0; i < data.Length; i++)
        {
            var frac = sym ? i / (data.Length - 1.0) : (double)i / data.Length;
            data[i] = (float)(a0 - a1 * Math.Cos(2.0 * Math.PI * frac));
        }

        return data;
    }

    public static int[] HammingTableQ31(int size, bool sym)
    {
        return HammingTableF32(size, sym).Select(x=>QConvert.ConvertToQ31(x, 0)).ToArray();
    }

    public static short[] HammingTableQ15(int size, bool sym)
    {
        return HammingTableF32(size, sym).Select(x => QConvert.ConvertToQ15(x, 0)).ToArray();
    }

    public static sbyte[] HammingTableQ7(int size, bool sym)
    {
        return HammingTableF32(size, sym).Select(x => QConvert.ConvertToQ7(x, 0)).ToArray();
    }
}