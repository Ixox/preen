/*
 * Copyright 2011 Xavier Hosxe
 *
 * Author: Xavier Hosxe (xavier.hosxe@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 /*
 * Oscillator waveform genration for PreenFM synth
 * http://www.preenfm.net
 */
 
import java.util.*;

class waveforms {
    enum WaveForm {
        SIN,
        SQUARE,
        SAWTOOTH
    };
    
    public static void main(String[] args) {
	    int numberOfSteps = 2048;
	    int order = 18;

	    if (args.length>0) {
		    try {
			    numberOfSteps = Integer.parseInt(args[0]);
		    } catch (java.lang.NumberFormatException e) {
			    System.out.println(args[0] + " is not a number");
			    printHelp();
			    return;
		    }
	    }

	    if (args.length>1) {
		    try {
			    order = Integer.parseInt(args[1]);
		    } catch (java.lang.NumberFormatException e) {
			    System.out.println(args[1] + " is not a number");
			    printHelp();
			    return;
		    }
    	}
    	
    	
    	System.out.println("const short sinTable[] __FLASH__ = {");
    	System.out.println(getWaveFormData(WaveForm.SIN, numberOfSteps, order));
	System.out.println("};");
	System.out.println("");
    	System.out.println("const short squareTable[] __FLASH__ = {");    	
    	System.out.println(getWaveFormData(WaveForm.SQUARE, numberOfSteps, order));
	System.out.println("};");
	System.out.println("");
    	System.out.println("const short sawTable[] __FLASH__ = {");    	
    	System.out.println(getWaveFormData(WaveForm.SAWTOOTH, numberOfSteps, order));
    	System.out.println("};");
    	
    }

    static String getWaveFormData(WaveForm type, int numberOfSteps, int order) {
        String nl = System.getProperty("line.separator");
        StringBuffer out = new StringBuffer();
	    List<Double> values = new ArrayList<Double>();
	    double max = 0;
	    double min = 1;
	    out.append(" // ================================================================= ").append(nl);
	    out.append(" // "+ type.toString() +" : order "+ order + " / "+ numberOfSteps +" steps").append(nl);
	    for (int i=0; i<numberOfSteps ; i++) {
		    double value = getValue(type, i, numberOfSteps, order);
		    if (value<min) {
			    min = value;
		    }
		    if (value>max) {
			    max = value;
		    }
    	    values.add(value);
	    }
        
        double m = max - min;
	    double mul = 65534 / m ;
	    int intMin = 0;
	    int intMax = 0;
	    int i = 0;
        out.append(" //  m  =  "+ m ).append(nl);
        out.append(" //  mul  =  "+ mul ).append(nl);
        out.append(" //  max  =  "+ max ).append(nl);
        out.append(" //  min  =  "+ min ).append(nl);
	    for (double value : values) {
		    int intValue = (int) ((value - min) * mul) - 32767;
		    if (intValue < intMin) {
			    intMin = intValue;
		    }
		    if (intValue > intMax) {
			    intMax = intValue;
		    }
		    out.append((int) intValue);
		    String stringValue = Integer.toString(intValue);
		    for (int s = 0; s<(6-stringValue.length()); s++) {
			    out.append(" ");
		    }
		    out.append(", ");
		    if ((i++ % 10)==9) {
    		    out.append(nl);
		    }			
	    }
	    out.append(nl);
	    out.append(" // Min =  "+ intMin );
	    out.append(" // Max =  "+ intMax );
	    out.append(nl);
        
        return out.toString();
    }

    static double getValue(WaveForm type, int i, int numberOfSteps, int order) {
	    double pi = 3.141592653589793238462;
	    double step = 2*pi/numberOfSteps * i;
	    double value = 0;
        switch (type) {
            case SAWTOOTH:
		        for (double k=1; k<=order; k++) {
			        value += 1/k* java.lang.Math.sin(step * k);
		        }
		        value = 1/2 - 1/pi * value;
                break;
            case SQUARE:
		        for (double k=1; k<=order; k+= 2) {
			        value += 1/k* java.lang.Math.sin(step * k);
		        }
		        value *= 4/pi;
                break;            
            case SIN:
                value = java.lang.Math.sin(step);
                break;
        }
        return value;
    }
    
	
	static void printHelp() {
		System.out.println("+-----------------------------------------");
		System.out.println("| Usage :  <steps> <order>");
		System.out.println("|    steps : number of steps");
        System.out.println("|    order : order of your waveforms");
		System.out.println("+-----------------------------------------");
	}

}
