/*
* Imagimob AB CONFIDENTIAL
* Unpublished Copyright (c) 2019- [Imagimob AB], All Rights Reserved.
* NOTICE: All information contained herein is, and remains the property of Imagimob AB.
*
* File created: 2020-09-05 Albert Seward (albert.seward@imagimob.com)
*/

using System;
using System.Linq;
using Imaginet.Viper.Object;

namespace Imaginet.Units.Signal.Hann;

public static class Hann
{
    /// <summary>
    /// 
    /// </summary>
    /// <param name="size"></param>
    /// <param name="sym"> When True, generates a symmetric window, for use in filter design. 
    /// When False, generates a periodic window, for use in spectral analysis.</param>
    /// <returns></returns>
    public static float[] HannTableF32(int size, bool sym)
    {
        var data = new float[size];

        for (int i = 0; i < data.Length; i++)
        {
            var frac = sym ? i / (data.Length - 1.0) : (double)i / data.Length;
            data[i] = (float)(0.5 - 0.5 * Math.Cos(2.0 * Math.PI * frac));
        }

        return data;
    }

    public static int[] HannTableQ31(int size, bool sym)
    {
        return HannTableF32(size, sym).Select(x => QConvert.ConvertToQ31(x, 0)).ToArray();
    }

    public static short[] HannTableQ15(int size, bool sym)
    {
        return HannTableF32(size, sym).Select(x => QConvert.ConvertToQ15(x, 0)).ToArray();
    }

    public static sbyte[] HannTableQ7(int size, bool sym)
    {
        return HannTableF32(size, sym).Select(x => QConvert.ConvertToQ7(x, 0)).ToArray();
    }
}