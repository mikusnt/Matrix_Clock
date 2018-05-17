/*
 * Copyright (C) 2018 MS-1
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package alphabet_generator;

import java.util.Arrays;
import java.util.Comparator;

/**
 *
 * @author MS-1
 */
public class ASCII_Char implements Comparator<ASCII_Char>, Comparable<ASCII_Char>, Cloneable {
    private static final int CODES_LENGTH = 5;
    public static final String SEP = ";";
    private char sign;
    private int[] codes = new int[CODES_LENGTH];
    private String description;
    private int id;
    private int modifiedDots;
    private int length;
    
    public ASCII_Char(int id) {
        this.id = id;
        this.sign = '?';
    }
    public ASCII_Char(int id, int[] codes) {
        this(id);
        setCodes(codes);    
    }
    public ASCII_Char(ASCII_Char original) {
        this.sign = original.getSign();
        this.codes = original.getCodes().clone();
        this.description = original.getDescription();
        this.id = original.getId();
        calculateDotsLength();
    }
    public ASCII_Char(ASCII_Char original, int newId) {
        this(original);
        this.id = newId;
    }
    private void calculateDotsLength() {
        modifiedDots = 0;
        for (int i = 0; i < CODES_LENGTH; i++) {
            int copy = codes[i];
            for (int j = 0; j < 8; j++) {
                if ((copy % 2) == 1) {
                    modifiedDots++;
                }
                copy >>= 1;
            }
        }
        if (codes[0] > 0) {
            if (codes[4] > 0)
                length = 5;
            else length = 4;
        } else if (codes[1] > 0) {
            if (codes[3] > 0)
                length = 3;
            else length = 2;
        } else if (codes[2] > 0) 
            length = 1;
        else length = 0;
    }

    public void setBit(int codeId, int bitId, boolean bitValue) throws ArrayIndexOutOfBoundsException {
        if (codeId >= CODES_LENGTH)
            throw new ArrayIndexOutOfBoundsException("Wrong number of array index named codeId: " + codeId + " required less than " + CODES_LENGTH);
        if (bitValue) {
            codes[codeId] |= (1 << bitId);
        } else {
            codes[codeId] &= ~(1 << bitId);
        }
        calculateDotsLength();
    }
    
    public String toCSVLine() {

        return String.valueOf(id) + SEP + String.valueOf(sign) + SEP + description + SEP + Arrays.toString(codes).replace(" ", "").replace("[", "").replace("]", "").replace(",", SEP) + "\n";
    }
    public static ASCII_Char fromCSVLine(String line) {
        
        String[] tokens = line.split(ASCII_Char.SEP);
        try {
            ASCII_Char out = new ASCII_Char(Integer.parseInt(tokens[0]));
            out.setSign(tokens[1].charAt(0));
            out.setDescription(tokens[2]);
            int[] bytes = new int[CODES_LENGTH];
            for (int i = 0; i < CODES_LENGTH; i++) {
                bytes[i] = Integer.parseInt(tokens[3+i]);
            }
            out.setCodes(bytes);
            return out;
        } catch (NumberFormatException e) {
            System.out.println("Error in parsing " + line + " to ASCII_Char object");
        }
        return null;
    }
    
    @Override
    public int compareTo(ASCII_Char e) {
        return id - e.getId();
    }
    @Override
    public int compare(ASCII_Char one, ASCII_Char two) {
        return one.compareTo(two);
    }
    
    /**
     * @return the sign
     */
    public char getSign() {
        return sign;
    }

    /**
     * @param sign the sign to set
     */
    public void setSign(char sign) {
        this.sign = sign;
    }

    /**
     * @return the codes
     */
    public int[] getCodes() {
        return codes;
    }

    /**
     * @param codes the codes to set
     */
    public final void setCodes(int[] codes) {
        if (codes.length == CODES_LENGTH)
            this.codes = codes;
        else throw new ArrayIndexOutOfBoundsException("Wrong number of array named codes: " + codes.length + " required: "+CODES_LENGTH);
        calculateDotsLength();
    }

    /**
     * @return the description
     */
    public String getDescription() {
        return description;
    }

    /**
     * @param description the description to set
     */
    public void setDescription(String description) {
        this.description = description;
    }

    /**
     * @return the id
     */
    public int getId() {
        return id;
    }

    /**
     * @return the modifiedDots
     */
    public int getModifiedDots() {
        return modifiedDots;
    }

    /**
     * @return the length
     */
    public int getLength() {
        return length;
    }
    
    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
