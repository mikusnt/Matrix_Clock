/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package alphabet_generator;

import java.awt.Color;
import java.awt.Label;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.ListSelectionModel;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author MS-1
 */
public class Main_Frame extends javax.swing.JFrame {
    private JCheckBox[][] checkBoxes;
    private JLabel[] labels;
    private int bytes[];
    private ASCII_List list;
    private int selectedRow;
    private String filename = "alphabet.csv";
    private boolean [][] checkEnable;
    /**
     * Creates new form Main_Frame
     */
    public Main_Frame() {
        initComponents();
        this.checkBoxes = new JCheckBox[][]{
            { jCheck_00, jCheck_01, jCheck_02, jCheck_03, jCheck_04, jCheck_05, jCheck_06, jCheck_07 },
            { jCheck_10, jCheck_11, jCheck_12, jCheck_13, jCheck_14, jCheck_15, jCheck_16, jCheck_17 },
            { jCheck_20, jCheck_21, jCheck_22, jCheck_23, jCheck_24, jCheck_25, jCheck_26, jCheck_27 },
            { jCheck_30, jCheck_31, jCheck_32, jCheck_33, jCheck_34, jCheck_35, jCheck_36, jCheck_37 },
            { jCheck_40, jCheck_41, jCheck_42, jCheck_43, jCheck_44, jCheck_45, jCheck_46, jCheck_47 }
        };
        this.checkEnable = new boolean[][] {
            { false, false, false, false, false },
            { false, false, true, false, false },
            { false, true, true, false, false },
            { false, true, true, true, false },
            { true, true, true, true, false },
            { true, true, true, true, true }
        };

        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 8; j++) {
                checkBoxes[i][j].addMouseListener(new MouseListener() {
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        checkboxClick();
                    }

                    @Override
                    public void mousePressed(MouseEvent e) {
                    }

                    @Override
                    public void mouseReleased(MouseEvent e) {
                    }

                    @Override
                    public void mouseEntered(MouseEvent e) {
                    }

                    @Override
                    public void mouseExited(MouseEvent e) {
                    }
                });
            }
        }
        
        this.bytes = new int[5];
        checkboxClick();
        jTableMain.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);


        list = ASCII_List.readFromCSV(filename);
        refreshList();

    }
    
    private void setCheckBoxColor() {
        if (jTableMain.getRowCount() > 0) {
            for(int i = 0; i < 5; i++) {     
                for(int j = 0; j < 8; j++) {
                    if (checkEnable[list.get(selectedRow).getLength()][i] == true) {
                        checkBoxes[i][j].setBackground(new Color(240, 240, 240));
                    } else 
                        checkBoxes[i][j].setBackground(Color.LIGHT_GRAY);

                }
            }
        }
    }
    
    private void loadASCII_List() {
        DefaultTableModel model = (DefaultTableModel)this.jTableMain.getModel();
        deleteAllRows(model);
        
        for (ASCII_Char item : list) {
            Vector row = new Vector();
            row.add(item.getId());
            row.add(item.getSign());
            row.add(item.getDescription());
            row.add(item.getModifiedDots());
            row.add(item.getLength());
            model.addRow(row);
        }
    }
    
    private void loadASCII_ListElement(int index) {
        ASCII_Char item = list.get(index);
        DefaultTableModel model = (DefaultTableModel)this.jTableMain.getModel();
        
        model.setValueAt(item.getId(), index, 0);
        model.setValueAt(String.valueOf(item.getSign()), index, 1);
        model.setValueAt(item.getDescription(), index, 2);
        model.setValueAt(item.getModifiedDots(), index, 3);    
        model.setValueAt(item.getLength(), index, 4);
    }
    public static void deleteAllRows(final DefaultTableModel model) {
        for( int i = model.getRowCount() - 1; i >= 0; i-- ) {
            model.removeRow(i);
        }
    }
    private void checkboxClick() {
        generateNumbers();
        numbersToText();
        if (jTableMain.getRowCount()>0) {
            int id = jTableMain.getSelectedRow();
            list.get(id).setCodes(bytes);
            loadASCII_ListElement(id);
        }
        setCheckBoxColor();
    }
    private void generateNumbers() {
        for(int i = 0; i < 5; i++) {
            bytes[i] = 0;
            for (int j = 0; j < 8; j++) {
                if (checkBoxes[i][j].isSelected())
                    bytes[i] += (1 << j);
            }
        }
    }
    private void numbersToText() {
        String str = "";
        for(int i = 0; i < 5; i++) {
            str += String.format("%3d", bytes[i])+" ";
        }
        jLabelNumbers.setText(str);
    }
    
    private void clearCheckBoxes() {
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 8; j++) {
                checkBoxes[i][j].setSelected(false);
            }
        }
    }
    
    private void refreshList() {
        list.saveToFile(filename);
        loadASCII_List();
        if (jTableMain.getRowCount() > 0) {
            jTableMain.setRowSelectionInterval(selectedRow, selectedRow);
            openSelectedItem();
        }
    }
    
    private void openSelectedItem() {
        int id = jTableMain.getSelectedRow();
        bytes = list.get(id).getCodes();
        for(int i = 0; i < 5; i++) {
            int copy = bytes[i];
            for(int j = 0; j < 8; j++) {
                checkBoxes[i][j].setSelected((copy % 2) == 1);
                copy >>= 1;
            }  
        }
        setCheckBoxColor();
        numbersToText();
    }
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        jTableMain = new javax.swing.JTable();
        jCheckPanel = new javax.swing.JPanel();
        jCheck_00 = new javax.swing.JCheckBox();
        jCheck_01 = new javax.swing.JCheckBox();
        jCheck_02 = new javax.swing.JCheckBox();
        jCheck_03 = new javax.swing.JCheckBox();
        jCheck_04 = new javax.swing.JCheckBox();
        jCheck_05 = new javax.swing.JCheckBox();
        jCheck_06 = new javax.swing.JCheckBox();
        jCheck_07 = new javax.swing.JCheckBox();
        jCheck_10 = new javax.swing.JCheckBox();
        jCheck_11 = new javax.swing.JCheckBox();
        jCheck_12 = new javax.swing.JCheckBox();
        jCheck_13 = new javax.swing.JCheckBox();
        jCheck_14 = new javax.swing.JCheckBox();
        jCheck_15 = new javax.swing.JCheckBox();
        jCheck_16 = new javax.swing.JCheckBox();
        jCheck_17 = new javax.swing.JCheckBox();
        jCheck_20 = new javax.swing.JCheckBox();
        jCheck_21 = new javax.swing.JCheckBox();
        jCheck_22 = new javax.swing.JCheckBox();
        jCheck_23 = new javax.swing.JCheckBox();
        jCheck_24 = new javax.swing.JCheckBox();
        jCheck_25 = new javax.swing.JCheckBox();
        jCheck_26 = new javax.swing.JCheckBox();
        jCheck_27 = new javax.swing.JCheckBox();
        jCheck_30 = new javax.swing.JCheckBox();
        jCheck_31 = new javax.swing.JCheckBox();
        jCheck_32 = new javax.swing.JCheckBox();
        jCheck_33 = new javax.swing.JCheckBox();
        jCheck_34 = new javax.swing.JCheckBox();
        jCheck_35 = new javax.swing.JCheckBox();
        jCheck_36 = new javax.swing.JCheckBox();
        jCheck_37 = new javax.swing.JCheckBox();
        jCheck_40 = new javax.swing.JCheckBox();
        jCheck_41 = new javax.swing.JCheckBox();
        jCheck_42 = new javax.swing.JCheckBox();
        jCheck_43 = new javax.swing.JCheckBox();
        jCheck_44 = new javax.swing.JCheckBox();
        jCheck_45 = new javax.swing.JCheckBox();
        jCheck_46 = new javax.swing.JCheckBox();
        jCheck_47 = new javax.swing.JCheckBox();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton3 = new javax.swing.JButton();
        jLabelNumbers = new javax.swing.JLabel();
        jButton4 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                formWindowClosing(evt);
            }
        });

        jTableMain.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "ID", "Znak", "Opis", "Kropki", "Długość"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.Integer.class, java.lang.String.class, java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class
            };
            boolean[] canEdit = new boolean [] {
                true, true, true, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jTableMain.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jTableMainMouseClicked(evt);
            }
        });
        jTableMain.addPropertyChangeListener(new java.beans.PropertyChangeListener() {
            public void propertyChange(java.beans.PropertyChangeEvent evt) {
                jTableMainPropertyChange(evt);
            }
        });
        jScrollPane1.setViewportView(jTableMain);
        if (jTableMain.getColumnModel().getColumnCount() > 0) {
            jTableMain.getColumnModel().getColumn(0).setMinWidth(50);
            jTableMain.getColumnModel().getColumn(0).setPreferredWidth(50);
            jTableMain.getColumnModel().getColumn(0).setMaxWidth(50);
            jTableMain.getColumnModel().getColumn(1).setMinWidth(50);
            jTableMain.getColumnModel().getColumn(1).setPreferredWidth(50);
            jTableMain.getColumnModel().getColumn(1).setMaxWidth(50);
            jTableMain.getColumnModel().getColumn(2).setResizable(false);
            jTableMain.getColumnModel().getColumn(3).setMinWidth(50);
            jTableMain.getColumnModel().getColumn(3).setPreferredWidth(50);
            jTableMain.getColumnModel().getColumn(3).setMaxWidth(50);
            jTableMain.getColumnModel().getColumn(4).setMinWidth(50);
            jTableMain.getColumnModel().getColumn(4).setPreferredWidth(50);
            jTableMain.getColumnModel().getColumn(4).setMaxWidth(50);
        }

        jCheck_06.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jCheck_06ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jCheckPanelLayout = new javax.swing.GroupLayout(jCheckPanel);
        jCheckPanel.setLayout(jCheckPanelLayout);
        jCheckPanelLayout.setHorizontalGroup(
            jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_00)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_10)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_20)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_30)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_40))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_01)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_11)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_21)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_31)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_41))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_02)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_12)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_22)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_32)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_42))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_03)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_13)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_23)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_33)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_43))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_04)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_14)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_24)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_34)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_44))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_05)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_15)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_25)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_35)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_45))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_06)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_16)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_26)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_36)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_46))
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addComponent(jCheck_07)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_17)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_27)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_37)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jCheck_47))
        );
        jCheckPanelLayout.setVerticalGroup(
            jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jCheckPanelLayout.createSequentialGroup()
                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(jCheckPanelLayout.createSequentialGroup()
                        .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(jCheckPanelLayout.createSequentialGroup()
                                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addGroup(jCheckPanelLayout.createSequentialGroup()
                                        .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                            .addGroup(jCheckPanelLayout.createSequentialGroup()
                                                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                                    .addGroup(jCheckPanelLayout.createSequentialGroup()
                                                        .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                                            .addGroup(jCheckPanelLayout.createSequentialGroup()
                                                                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                                    .addComponent(jCheck_00)
                                                                    .addComponent(jCheck_10)
                                                                    .addComponent(jCheck_20)
                                                                    .addComponent(jCheck_30)
                                                                    .addComponent(jCheck_40))
                                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                                                    .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                                        .addComponent(jCheck_01)
                                                                        .addComponent(jCheck_21, javax.swing.GroupLayout.Alignment.TRAILING))
                                                                    .addComponent(jCheck_31)
                                                                    .addComponent(jCheck_41)))
                                                            .addComponent(jCheck_11))
                                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                        .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                                            .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                                .addComponent(jCheck_02)
                                                                .addComponent(jCheck_22, javax.swing.GroupLayout.Alignment.TRAILING))
                                                            .addComponent(jCheck_32)
                                                            .addComponent(jCheck_42)))
                                                    .addComponent(jCheck_12))
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                                    .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                        .addComponent(jCheck_03)
                                                        .addComponent(jCheck_23, javax.swing.GroupLayout.Alignment.TRAILING))
                                                    .addComponent(jCheck_33)
                                                    .addComponent(jCheck_43)))
                                            .addComponent(jCheck_13))
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                            .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                .addComponent(jCheck_04)
                                                .addComponent(jCheck_24, javax.swing.GroupLayout.Alignment.TRAILING))
                                            .addComponent(jCheck_34)
                                            .addComponent(jCheck_44)))
                                    .addComponent(jCheck_14))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                        .addComponent(jCheck_05)
                                        .addComponent(jCheck_25, javax.swing.GroupLayout.Alignment.TRAILING))
                                    .addComponent(jCheck_35)
                                    .addComponent(jCheck_45)))
                            .addComponent(jCheck_15))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(jCheck_06)
                                .addComponent(jCheck_26, javax.swing.GroupLayout.Alignment.TRAILING))
                            .addComponent(jCheck_36)
                            .addComponent(jCheck_46)))
                    .addComponent(jCheck_16))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jCheckPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jCheck_07)
                    .addComponent(jCheck_17)
                    .addComponent(jCheck_27)
                    .addComponent(jCheck_37)
                    .addComponent(jCheck_47)))
        );

        jButton1.setLabel("Wyczyść");
        jButton1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton1MouseClicked(evt);
            }
        });

        jButton2.setText("Dodaj");
        jButton2.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton2MouseClicked(evt);
            }
        });

        jButton3.setText("Usuń");
        jButton3.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton3MouseClicked(evt);
            }
        });

        jLabelNumbers.setFont(new java.awt.Font("Courier New", 1, 9)); // NOI18N
        jLabelNumbers.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        jLabelNumbers.setText("jLabel5");

        jButton4.setLabel("Zapisz do pliku");
        jButton4.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jButton4MouseClicked(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jButton3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jButton2))
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 305, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jButton1)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(jCheckPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jLabelNumbers, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(jButton4))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 282, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(35, 35, 35)
                        .addComponent(jCheckPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabelNumbers)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jButton1)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jButton2)
                        .addComponent(jButton4))
                    .addComponent(jButton3))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButton1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MouseClicked
        if (JOptionPane.showConfirmDialog(
        this,
        "Czy na pewno?",
        "Pytanie - czyszczenie okna",
        JOptionPane.YES_NO_OPTION) == 0) {
            clearCheckBoxes();
            checkboxClick();
        }
    }//GEN-LAST:event_jButton1MouseClicked

    private void jTableMainMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jTableMainMouseClicked
        selectedRow = jTableMain.getSelectedRow();
        loadASCII_ListElement(selectedRow);
        openSelectedItem();
    }//GEN-LAST:event_jTableMainMouseClicked

    private void jCheck_06ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jCheck_06ActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_jCheck_06ActionPerformed

    private void jButton2MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton2MouseClicked
        selectedRow = jTableMain.getSelectedRow();
        int newId = list.getNextEmptyId(jTableMain.getSelectedRow());
        try {
            list.tryAdd(new ASCII_Char(newId));
        } catch (IllegalAccessException e) {
            System.out.println(e.toString());
        }
        refreshList();
    }//GEN-LAST:event_jButton2MouseClicked

    private void jButton3MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton3MouseClicked
        list.remove(jTableMain.getSelectedRow());
        refreshList();
    }//GEN-LAST:event_jButton3MouseClicked

    private void jButton4MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton4MouseClicked
        list.saveToFile(filename);
    }//GEN-LAST:event_jButton4MouseClicked

    private void formWindowClosing(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_formWindowClosing
        list.saveToFile(filename);
    }//GEN-LAST:event_formWindowClosing

    private void jTableMainPropertyChange(java.beans.PropertyChangeEvent evt) {//GEN-FIRST:event_jTableMainPropertyChange
        if (jTableMain.getRowCount() > 0) {
            if (list.get(selectedRow).getSign() != ((String)jTableMain.getValueAt(selectedRow, 1)).charAt(0)) {
                System.out.println("different chars");
                list.get(selectedRow).setSign(((String)jTableMain.getValueAt(selectedRow, 1)).charAt(0));
            }
            if (list.get(selectedRow).getDescription() != (String)jTableMain.getValueAt(selectedRow, 2)) {
                System.out.println("different description");
                list.get(selectedRow).setDescription((String)jTableMain.getValueAt(selectedRow, 2));
            }
            if (list.get(selectedRow).getId() != (int)jTableMain.getValueAt(selectedRow, 0)) {
                int newId = (int)jTableMain.getValueAt(selectedRow, 0);
                list.renameItemId(selectedRow, newId);
                refreshList();
            }
                

        }
    }//GEN-LAST:event_jTableMainPropertyChange

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Main_Frame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Main_Frame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Main_Frame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Main_Frame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Main_Frame().setVisible(true);
            }
        });
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JPanel jCheckPanel;
    private javax.swing.JCheckBox jCheck_00;
    private javax.swing.JCheckBox jCheck_01;
    private javax.swing.JCheckBox jCheck_02;
    private javax.swing.JCheckBox jCheck_03;
    private javax.swing.JCheckBox jCheck_04;
    private javax.swing.JCheckBox jCheck_05;
    private javax.swing.JCheckBox jCheck_06;
    private javax.swing.JCheckBox jCheck_07;
    private javax.swing.JCheckBox jCheck_10;
    private javax.swing.JCheckBox jCheck_11;
    private javax.swing.JCheckBox jCheck_12;
    private javax.swing.JCheckBox jCheck_13;
    private javax.swing.JCheckBox jCheck_14;
    private javax.swing.JCheckBox jCheck_15;
    private javax.swing.JCheckBox jCheck_16;
    private javax.swing.JCheckBox jCheck_17;
    private javax.swing.JCheckBox jCheck_20;
    private javax.swing.JCheckBox jCheck_21;
    private javax.swing.JCheckBox jCheck_22;
    private javax.swing.JCheckBox jCheck_23;
    private javax.swing.JCheckBox jCheck_24;
    private javax.swing.JCheckBox jCheck_25;
    private javax.swing.JCheckBox jCheck_26;
    private javax.swing.JCheckBox jCheck_27;
    private javax.swing.JCheckBox jCheck_30;
    private javax.swing.JCheckBox jCheck_31;
    private javax.swing.JCheckBox jCheck_32;
    private javax.swing.JCheckBox jCheck_33;
    private javax.swing.JCheckBox jCheck_34;
    private javax.swing.JCheckBox jCheck_35;
    private javax.swing.JCheckBox jCheck_36;
    private javax.swing.JCheckBox jCheck_37;
    private javax.swing.JCheckBox jCheck_40;
    private javax.swing.JCheckBox jCheck_41;
    private javax.swing.JCheckBox jCheck_42;
    private javax.swing.JCheckBox jCheck_43;
    private javax.swing.JCheckBox jCheck_44;
    private javax.swing.JCheckBox jCheck_45;
    private javax.swing.JCheckBox jCheck_46;
    private javax.swing.JCheckBox jCheck_47;
    private javax.swing.JLabel jLabelNumbers;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTable jTableMain;
    // End of variables declaration//GEN-END:variables
}
