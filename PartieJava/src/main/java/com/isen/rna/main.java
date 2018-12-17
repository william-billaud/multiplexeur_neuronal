package com.isen.rna;

import com.bitsinharmony.recognito.Recognito;
import com.bitsinharmony.recognito.VoicePrint;

import javax.sound.sampled.UnsupportedAudioFileException;
import java.io.File;
import java.io.IOException;
import com.csvwritter.CSVUtils;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;

public class main {

    public static void main(String[] args){
        Recognito<String> recognito = new Recognito<>(8000.0f);
        VoicePrint print = null;
        String csvFile = args[0]+"data.csv";
        FileWriter writer = null;
        try {
            writer = new FileWriter(csvFile);
        } catch (IOException e) {
            e.printStackTrace();
        }
        for(int i = 1;i<=5;i++){
            String path = args[0]+ i + "/";
            System.out.println(path);
            File rep = new File( path );
            File[] files  = rep.listFiles();
            assert files != null;
            for (File f: files ) {
                System.out.println(f.getName());
                try {
                    print = recognito.createVoicePrint(f.getName() + Integer.toString(i), f);
                } catch (UnsupportedAudioFileException | IOException e) {
                    e.printStackTrace();
                }
                if (print != null) {
                    System.out.println(print.toString());
                        List<String> data = new ArrayList<String>();
                        for (Double d : print.getFeatures()) {
                            data.add(Double.toString(d));
                        }
                        data.add(Integer.toString(i));
                    try {
                        CSVUtils.writeLine(writer, data, ';');
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
        try {
            assert writer != null;
            writer.flush();
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
