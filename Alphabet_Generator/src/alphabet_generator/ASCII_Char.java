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
    private char sign;
    private byte[] codes = new byte[CODES_LENGTH];
    private String description;
    private int id;
    
    public ASCII_Char(int id) {
        this.id = id;
        this.description = "empty";
        this.sign = '?';
    }
    public ASCII_Char(int id, byte[] codes) {
        this(id);
        setCodes(codes);    
    }

    public void setBit(int codeId, int bitId, boolean bitValue) throws ArrayIndexOutOfBoundsException {
        if (codeId >= CODES_LENGTH)
            throw new ArrayIndexOutOfBoundsException("Wrong number of array index named codeId: " + codeId + " required less than " + CODES_LENGTH);
        if (bitValue) {
            codes[codeId] |= (1 << bitId);
        } else {
            codes[codeId] &= ~(1 << bitId);
        }
    }
    
    public String toCSVLine() {
        String sep = ";";
        return String.valueOf(id) + sep + String.valueOf(sign) + sep + description + sep + Arrays.toString(codes).replace(" ", "").replace("[", "").replace("]", "").replace(",", sep);
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
    public byte[] getCodes() {
        return codes;
    }

    /**
     * @param codes the codes to set
     */
    public final void setCodes(byte[] codes) {
        if (codes.length == CODES_LENGTH)
            this.codes = codes;
        else throw new ArrayIndexOutOfBoundsException("Wrong number of array named codes: " + codes.length + " required: "+CODES_LENGTH);
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
    
    
}
