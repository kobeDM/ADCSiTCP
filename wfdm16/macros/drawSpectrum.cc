#include "inc/shinclude.h"

void drawSpectrum( const String& inputFile, const String& outputDir )
{
    SetShStyle( );
    ShUtil::ExistCreateDir( outputDir );

    TFile file( inputFile.c_str( ) );
    TTree* pTree = dynamic_cast< TTree* >( file.Get( "tree" ) );
    if( pTree == nullptr ) return;

    int   fadcVar[8][2048] = {};
    pTree->SetBranchAddress( "fadc0", fadcVar[0] );
    pTree->SetBranchAddress( "fadc1", fadcVar[1] );
    pTree->SetBranchAddress( "fadc2", fadcVar[2] );
    pTree->SetBranchAddress( "fadc3", fadcVar[3] );
    pTree->SetBranchAddress( "fadc4", fadcVar[4] );
    pTree->SetBranchAddress( "fadc5", fadcVar[5] );
    pTree->SetBranchAddress( "fadc6", fadcVar[6] );
    pTree->SetBranchAddress( "fadc7", fadcVar[7] );
    
    std::vector< TH1F* > histPHArr;
    std::vector< TH1F* > histCIArr;
    histPHArr.reserve( 8 );
    histCIArr.reserve( 8 );
    for( int i = 0; i < 8; ++i ) {
        String histPHName = Form( "hist_PH_%d", i );
        String histCIName = Form( "hist_CI_%d", i );
        histPHArr.push_back( new TH1F( histPHName.c_str( ), histPHName.c_str( ), 100, -400.0, 1100.0 ) );
        histCIArr.push_back( new TH1F( histCIName.c_str( ), histCIName.c_str( ), 100, -50000.0, 150000.0 ) );
    }

    TH1F* pHistAllPH = new TH1F( "hist_PH_All", "hist_PH_All", 100, -50.0, 950.0 );
    TH1F* pHistAllCI = new TH1F( "hist_CI_All", "hist_CI_All", 100, -50000.0, 150000.0 );

    TFile outFile( Form( "%s/output.root", outputDir.c_str( ) ), "RECREATE" );
    TTree* pOutTree = new TTree( "tree", "tree" );
    pOutTree->SetDirectory( &outFile );

    double maxHightArr[8]  = {};
    double intChargeArr[8] = {};
    pOutTree->Branch( "maxHight0", &maxHightArr[0] );
    pOutTree->Branch( "maxHight1", &maxHightArr[1] );
    pOutTree->Branch( "maxHight2", &maxHightArr[2] );
    pOutTree->Branch( "maxHight3", &maxHightArr[3] );
    pOutTree->Branch( "maxHight4", &maxHightArr[4] );
    pOutTree->Branch( "maxHight5", &maxHightArr[5] );
    pOutTree->Branch( "maxHight6", &maxHightArr[6] );
    pOutTree->Branch( "maxHight7", &maxHightArr[7] );
    pOutTree->Branch( "intCharge0", &intChargeArr[0] );
    pOutTree->Branch( "intCharge1", &intChargeArr[1] );
    pOutTree->Branch( "intCharge2", &intChargeArr[2] );
    pOutTree->Branch( "intCharge3", &intChargeArr[3] );
    pOutTree->Branch( "intCharge4", &intChargeArr[4] );
    pOutTree->Branch( "intCharge5", &intChargeArr[5] );
    pOutTree->Branch( "intCharge6", &intChargeArr[6] );
    pOutTree->Branch( "intCharge7", &intChargeArr[7] );

    int totEvt = pTree->GetEntries( );
    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );
        
        double allMaxHight  = 0.0;
        double allIntCharge = 0.0;
        for( int ch = 0; ch < 8; ++ch ) {
            TH1F* pHistPH = histPHArr.at( ch );
            TH1F* pHistCI = histCIArr.at( ch );
            if( pHistPH == nullptr || pHistCI == nullptr ) continue;

            double pedestal = 0.0;
            for( int clk = 0; clk < 400; ++clk )
                pedestal += static_cast< double >( fadcVar[ch][clk] );

            pedestal /= 400.0;

            // DEBUG(pedestal);
            double maxHight = pedestal;
            double intCharge = 0.0;;
            // for( int clk = 400; clk < 2048; ++clk ) {
            for( int clk = 400; clk < 1000; ++clk ) {
                if( fadcVar[ch][clk] > maxHight ) maxHight = static_cast< double >( fadcVar[ch][clk] );
                intCharge += ( static_cast< double >( fadcVar[ch][clk] ) - pedestal );
            }
            pHistPH->Fill( maxHight - pedestal );
            pHistCI->Fill( intCharge );

            allMaxHight += ( maxHight - pedestal );
            allIntCharge += intCharge;
            maxHightArr[ch] = maxHight - pedestal;
            intChargeArr[ch] = intCharge;
        }

        pOutTree->Fill( );
        // DEBUG(allMaxHight);
        pHistAllPH->Fill( allMaxHight  );
        pHistAllCI->Fill( allIntCharge );
    }

    pOutTree->Write( );

    TF1 func("gaus","gaus", 0, 200000);
    func.SetNpx(2000);
    func.SetLineWidth(2);
    TCanvas cvs( "cvs", "cvs", 800, 800 );
    cvs.Divide( 1, 2 );
    for( int ch = 0; ch < 8; ++ch ) {
        cvs.cd( 1 );
        TH1F* pHistPH = histPHArr.at( ch );
        if( pHistPH == nullptr ) continue;
        pHistPH->GetXaxis()->SetTitle( "ADC count" );
        pHistPH->GetYaxis()->SetTitle( "Events" );
        pHistPH->Draw( );
        pHistPH->Fit( &func,"MIQ", "", 30, 300 );
        // ShTUtil::CreateDrawText(0.2, 0.88, Form("Channel %d", ch));
        // ShTUtil::CreateDrawText(0.2, 0.78, "Peak hight: 500 mV");
        // ShTUtil::CreateDrawText(0.2, 0.7, Form("Mean: %2.1lf #pm %2.1lf", func.GetParameter(1), func.GetParError(1)));

        // ShTUtil::CreateDrawText(0.2, 0.5, Form("Cal. factor: %2.2lf mV/ADC", 500.0 / func.GetParameter(1)));

        ShTUtil::CreateDrawText(0.6, 0.88, Form("Channel %d", ch));
        ShTUtil::CreateDrawText(0.6, 0.8, Form("Mean: %2.1lf #pm %2.1lf", func.GetParameter(1), func.GetParError(1)));

        cvs.cd( 2 );
        TH1F* pHistCI = histCIArr.at( ch );
        if( pHistCI == nullptr ) continue;
        pHistCI->GetXaxis()->SetTitle( "ADC count #times clock" );
        pHistCI->GetYaxis()->SetTitle( "Events" );
        pHistCI->Draw( );
        pHistCI->Fit( &func,"MIQ", "", 10000, 60000 );
        // ShTUtil::CreateDrawText(0.2, 0.88, Form("Channel %d", ch));
        // ShTUtil::CreateDrawText(0.2, 0.78, "Charge: 1000 fC");
        // ShTUtil::CreateDrawText(0.2, 0.7, Form("Mean: %2.1lf #pm %2.1lf", func.GetParameter(1), func.GetParError(1)));

        // ShTUtil::CreateDrawText(0.2, 0.5, Form("Cal. factor: %2.4lf fC/(ADC #times Clock)", 1000 / func.GetParameter(1)));


        ShTUtil::CreateDrawText(0.6, 0.88, Form("Channel %d", ch));
        ShTUtil::CreateDrawText(0.6, 0.8, Form("Mean: %2.1lf #pm %2.1lf", func.GetParameter(1), func.GetParError(1)));


        cvs.SaveAs( Form( "%s/ch_%d.png", outputDir.c_str( ), ch ) );
        cvs.SaveAs( Form( "%s/ch_%d.pdf", outputDir.c_str( ), ch ) );
        cvs.SaveAs( Form( "%s/ch_%d.eps", outputDir.c_str( ), ch ) );
    }

    cvs.cd( 1 );
    pHistAllPH->GetXaxis()->SetTitle( "ADC count" );
    pHistAllPH->GetYaxis()->SetTitle( "Events" );
    pHistAllPH->Draw( );
    // pHistAllPH->Fit( &func,"MIQ" );
    // ShTUtil::CreateDrawText(0.2, 0.88, Form("Channel %d", ch));
    // ShTUtil::CreateDrawText(0.2, 0.78, "Peak hight: 500 mV");
    // ShTUtil::CreateDrawText(0.2, 0.7, Form("Mean: %2.1lf #pm %2.1lf", func.GetParameter(1), func.GetParError(1)));

    // ShTUtil::CreateDrawText(0.2, 0.5, Form("Cal. factor: %2.2lf mV/ADC", 500.0 / func.GetParameter(1)));


    cvs.cd( 2 );
    pHistAllCI->GetXaxis()->SetTitle( "ADC count #times clock" );
    pHistAllCI->GetYaxis()->SetTitle( "Events" );
    pHistAllCI->Draw( );
    // pHistAllCI->Fit( &func,"MIQ" );
    // ShTUtil::CreateDrawText(0.2, 0.88, Form("Channel %d", ch));
    // ShTUtil::CreateDrawText(0.2, 0.78, "Charge: 1000 fC");
    // ShTUtil::CreateDrawText(0.2, 0.7, Form("Mean: %2.1lf #pm %2.1lf", func.GetParameter(1), func.GetParError(1)));

    // ShTUtil::CreateDrawText(0.2, 0.5, Form("Cal. factor: %2.4lf fC/(ADC #times Clock)", 1000 / func.GetParameter(1)));

    cvs.SaveAs( Form( "%s/all.png", outputDir.c_str( ) ) );
    cvs.SaveAs( Form( "%s/all.pdf", outputDir.c_str( ) ) );
    cvs.SaveAs( Form( "%s/all.eps", outputDir.c_str( ) ) );


    return;
}
