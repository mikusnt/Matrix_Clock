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
public class ASCII_Char implements Comparator<ASCII_Char>, Comparable<ASCII_Char> {
    private static final int CODES_LENGTH = 5;
    public static final String SEP = ";";
    private char sign;
    private int[] codes = new int[CODES_LENGTH];
    private String description;
    private int id;
    private int modifiedDots;
    
    public ASCII_Char(int id) {
        this.id = id;
        this.description = "empty";
        this.sign = '?';
    }
    public ASCII_Char(int id, int[] codes) {
        this(id);
        setCodes(codes);    
    }
    
    private void calculateModifiedDots() {
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
    }

    public void setBit(int codeId, int bitId, boolean bitValue) throws ArrayIndexOutOfBoundsException {
        if (codeId >= CODES_LENGTH)
            throw new ArrayIndexOutOfBoundsException("Wrong number of array index named codeId: " + codeId + " required less than " + CODES_LENGTH);
        if (bitValue) {
            codes[codeId] |= (1 << bitId);
        } else {
            codes[codeId] &= ~(1 << bitId);
        }
        calculateModifiedDots();
    }
    
    public String toCSVLine() {

        return String.valueOf(id) + SEP + String.valueOf(sign) + SEP + description + SEP + Arrays.toString(codes).replace(" ", "").replace("[", "").replace("]", "").replace(",", SEP) + "\n";
    }
    public static ASCII_Char fromCSVLine(String line) {
        ASCII_Char out = new ASCII_Char(0);
        String[] tokens = line.split(ASCII_Char.SEP);
        try {
            out.setId(Integer.parseInt(tokens[0]));
            out.setSign(tokens[1].charAt(0));
            out.setDescription(tokens[2]);
            int[] bytes = new int[CODES_LENGTH];
            for (int i = 0; i < CODES_LENGTH; i++) {
                bytes[i] = Integer.parseInt(tokens[3+i]);
            }
            out.setCodes(bytes);
        } catch (NumberFormatException e) {
            System.out.println("Error in parsing " + line + " to ASCII_Char object");
        }
        return out;
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
        calculateModifiedDots();
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
     * @param id the id to set
     */
    public void setId(int id) {
        this.id = id;
    }

    /**
     * @return the modifiedDots
     */
    public int getModifiedDots() {
        return modifiedDots;
    }
    
    
}
