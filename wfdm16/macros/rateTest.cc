#include "inc/shinclude.h"

void rateTest( const String& inputTxt, const String& outputFile )
{
    SetShStyle( );

    std::ifstream ifs;
    ifs.open( inputTxt );
    if( ifs.is_open( ) == false ) return;

    std::map< int, TH1F* > histTable;

    int    time = 0;
    while( !ifs.eof( ) ) {
        std::string line = "";
        std::getline( ifs, line );
        if( line.length( ) <= 0 ) continue;
        if( strncmp( line.c_str( ), "#", 1 ) == 0 ) {
            StringStream ss( line );
            String tmp = "";
            ss >> tmp >> time;
            if( histTable.find( time ) == histTable.end( ) ) {
                String histName = Form( "hist_%d", time );
                TH1F* pHist = new TH1F( histName.c_str( ), histName.c_str( ), 200, 0, 200 );
                histTable.insert( std::make_pair( time, pHist ) );
            }
            continue;
        }

        TH1F* pHist = histTable.at( time  );
        if( pHist == nullptr ) continue;

        double entries;
        StringStream ss( line );
        ss >> entries;
        double rate = entries / static_cast< double >( time );

        pHist->Fill( rate );
    }

    int    nPoints              = histTable.size( );
    double trigNum    [100] = { };
    double meanRate   [100] = { };
    double trigNumErr [100] = { };
    double meanRateErr[100] = { };
    int idx = 0;
    for( auto pair : histTable ) {
        int   num   = pair.first;
        TH1F* pHist = pair.second;
        if( pHist == nullptr ) continue;
        trigNum    [idx] = num;
        meanRate   [idx] = pHist->GetMean( );
        trigNumErr [idx] = 0.0001;
        meanRateErr[idx] = pHist->GetRMS( );
        
        ++idx;
    }
    
    TGraphErrors graph( nPoints, trigNum, meanRate, trigNumErr, meanRateErr );
    
    TCanvas cvs( "cvs", "cvs", 800, 600 );
    graph.Draw( "AP" );
    graph.GetXaxis()->SetTitle( "DAQ time" );
    graph.GetYaxis()->SetTitle( "Trigger rate [Hz]" );
    graph.GetYaxis()->SetRangeUser( 0.0, 200.0 );

    cvs.SaveAs( Form( "%s.png", outputFile.c_str( ) ) );
    
    return;
}
  
