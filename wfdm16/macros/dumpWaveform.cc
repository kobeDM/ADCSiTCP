#include "inc/shinclude.h"

const int    CH_TOTAL    = 16;
const String FILE_PREFIX = "ch";
const String FILE_EXT    = "txt";

void dumpWaveform( const String& inputDir, const int& evtID )
{
    SetAtlasStyle( );

    TCanvas cvs( "cvs", "cvs", 800, 600 );
    for( int idx = 0; idx < CH_TOTAL; ++idx ) {
        String fileName = Form( "%s/%s_%d.%s",
                                inputDir.c_str( ),
                                FILE_PREFIX.c_str( ),
                                idx,
                                FILE_EXT.c_str( ) );
        std::ifstream ifs;
        ifs.open( fileName );
        if( ifs.is_open( ) == false ) continue;

        int adcArr [200] = {}; // actually the depth should be less than 128
        int timeArr[200] = {}; // actually the depth should be less than 128
    
        int trig = 0;
        int adc = 0;
        int cnt = 0;
        int max = -100, min = 10000;
        while( !ifs.eof( ) ) {
            std::string line = "";
            std::getline( ifs, line );
            if( line.length( ) <= 0 || strncmp( line.c_str( ), "#", 1 ) == 0 ) continue;

            std::stringstream ss( line );
            ss >> trig >> adc;

            if( trig == evtID ) {
                adcArr[cnt] = adc;
                timeArr[cnt] = cnt;
                cnt++;

                if( max < adc ) max = adc;
                if( min > adc ) min = adc;
            }
        }

        TGraph graph( cnt, timeArr, adcArr );
        graph.Draw("AP");
        graph.GetXaxis()->SetTitle( "Clock [5 MHz]" );
        graph.GetYaxis()->SetTitle( "ADC count" );
        graph.GetXaxis()->SetRangeUser( 0, 128 );
        graph.GetYaxis()->SetRangeUser( min - 100 , max + 100 );
        
        String saveFileBase = Form( "%s/out_%s_%d", inputDir.c_str( ), FILE_PREFIX.c_str( ), idx );
        cvs.SaveAs( Form( "%s.png", saveFileBase.c_str( ) ) );
    }
    
    return;
}
