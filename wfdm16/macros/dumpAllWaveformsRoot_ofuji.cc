#include "inc/shinclude.h"

void dumpAllWaveformsRoot_ofuji( const String& inputFile)
{
    SetShStyle( );

    std::vector< TH2F* > histArr;
    std::vector< TH1F* > maxArr;
    std::vector< TH1F* > minArr;
    std::vector< TH1F* > intArr;
    std::vector< TH2F* > max_intArr;
    std::vector< TH2F* > histArrcut;
    std::vector< TH1F* > maxArrcut;
    std::vector< TH1F* > intArrcut;
    std::vector< TH2F* > max_intArrcut;
    histArr.reserve( 8 );
    maxArr.reserve( 8 );
    minArr.reserve( 8 );
    intArr.reserve( 8 );
    max_intArr.reserve( 8 );
    histArrcut.reserve( 8 );
    maxArrcut.reserve( 8 );
    intArrcut.reserve( 8 );
    max_intArrcut.reserve( 8 );

    for( int i = 0; i < 8; ++i ) {
        String histName = Form( "hist_%d", i );
        String maxName = Form( "max_%d", i );
        String minName = Form( "min_%d", i );
        String intName = Form( "int_%d", i );
        String max_intName = Form( "max_int_%d", i );
        String histNamecut = Form( "hist_cut_%d", i );
        String maxNamecut = Form( "max_cut_%d", i );
        String intNamecut = Form( "int_cut_%d", i );
        String max_intNamecut = Form( "max_int_cut_%d", i );

        histArr.push_back( new TH2F( histName.c_str( ), histName.c_str( ), 4096, 0.0, 4096.0, 4096, -2096.0, 2000.0 ) );
        maxArr.push_back( new TH1F( maxName.c_str( ), maxName.c_str( ), 4096, -2096.0, 2000.0) );
        minArr.push_back( new TH1F( minName.c_str( ), minName.c_str( ), 4096, -2096.0, 2000.0) );
        intArr.push_back( new TH1F( intName.c_str( ), intName.c_str( ), 300, -20000.0, 40000.0) );
        max_intArr.push_back( new TH2F( max_intName.c_str( ), max_intName.c_str( ), 4096, -2096.0, 2000.0, 300, -20000.0, 40000.0) );
        histArrcut.push_back( new TH2F( histNamecut.c_str( ), histNamecut.c_str( ), 4096, 0.0, 4096.0, 4096, -2096.0, 2000.0 ) );
        maxArrcut.push_back( new TH1F( maxNamecut.c_str( ), maxNamecut.c_str( ), 4096, -2096.0, 2000.0) );
        intArrcut.push_back( new TH1F( intNamecut.c_str( ), intNamecut.c_str( ), 300, -20000.0, 40000.0) );
        max_intArrcut.push_back( new TH2F( max_intNamecut.c_str( ), max_intNamecut.c_str( ), 4096, -2096.0, 2000.0, 300, -20000.0, 40000.0) );
    }

    TFile file( inputFile.c_str( ) );
    TTree* pTree = dynamic_cast< TTree* >( file.Get( "tree" ) );
    if( pTree == nullptr ) return;

    int   fadcVar[8][4096] = {};
    pTree->SetBranchAddress( "fadc0", fadcVar[0] );
    pTree->SetBranchAddress( "fadc1", fadcVar[1] );
    pTree->SetBranchAddress( "fadc2", fadcVar[2] );
    pTree->SetBranchAddress( "fadc3", fadcVar[3] );
    pTree->SetBranchAddress( "fadc4", fadcVar[4] );
    pTree->SetBranchAddress( "fadc5", fadcVar[5] );
    pTree->SetBranchAddress( "fadc6", fadcVar[6] );
    pTree->SetBranchAddress( "fadc7", fadcVar[7] );

    vector<int > pre_mean( 8 );
    vector<int > pre_max( 8 );
    vector<int > pre_min( 8 );
    vector<int > pre_count( 8 );
    vector<int > pre_mean_cut( 8 );
    vector<int > pre_max_cut( 8 );
    vector<int > pre_count_cut( 8 );

    int totEvt = pTree->GetEntries( );
    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );

        for( int ch = 0; ch < 8; ++ch ) {
            TH2F* pHist = histArr.at( ch );
            TH1F* pMax = maxArr.at( ch );
            TH1F* pMin = minArr.at( ch );
            TH1F* pInt = intArr.at( ch );
            TH2F* pMax_Int = max_intArr.at( ch );
            TH2F* pHistcut = histArrcut.at( ch );
            TH1F* pMax_cut = maxArrcut.at( ch );
            TH1F* pInt_cut = intArrcut.at( ch );
            TH2F* pMax_Int_cut = max_intArrcut.at( ch );
            if( pHist == nullptr ) continue;
            double base_fadc = 0.;
            double max_fadc = 0.;
            double min_fadc = 0.;
            double sum_fadc = 0.;
            int max_clock = 0;
            int min_clock = 0;
            bool fadc_over30flag = false;
            bool fadc_under10flag = false;
            for( int clk = 2; clk < 4096; ++clk ) {
                if(fadcVar[ch][clk]>2000){
                    fadcVar[ch][clk]-=4096;
                }
                if(clk < 200){
                    base_fadc += fadcVar[ch][clk];
                }
            }
            base_fadc = base_fadc/198;
    	    //if(evt%1000==0){
	    //	cout << "ch" << ch << " ,  offset=" << base_fadc <<endl;
	    //}
            for( int clk = 2; clk < 4096; ++clk ) {
                sum_fadc += fadcVar[ch][clk] - base_fadc;
                fadcVar[ch][clk]-=base_fadc;
		if(clk>=420&&clk<=520){
                if(max_fadc<=fadcVar[ch][clk]){
                    max_fadc = fadcVar[ch][clk]; 
                    max_clock = clk;
                }
                if(min_fadc>=fadcVar[ch][clk]){
                    min_fadc = fadcVar[ch][clk]; 
                    min_clock = clk;
                }
		}
                if((clk<1450||1620<clk)&&fadcVar[ch][clk]>=30){
                    fadc_over30flag = true;
                }
                if((1500<=clk&&clk<=1550)&&fadcVar[ch][clk]<10){
                    fadc_under10flag = true;
                }
            }
            pMax->Fill(max_fadc);
            pMin->Fill(min_fadc);
            pInt->Fill(sum_fadc);
            pMax_Int->Fill(max_fadc,sum_fadc);
            pre_mean[ch] += sum_fadc;	
            pre_max[ch] += max_fadc;	
            pre_count[ch]++;
            for( int clk = 2; clk < 4096; ++clk ) {
                pHist->Fill( clk, fadcVar[ch][clk] );
            }

            //==============
            //cut hist start
            //==============
            
            //if(max_fadc>20&&(1450<=max_clock&&max_clock<=1620)&&fadc_over30flag==false){
            //if(max_fadc>20&&(1500<=max_clock&&max_clock<=1600)&&fadc_over30flag==false){
            if(max_fadc>20&&(1500<=max_clock&&max_clock<=1600)&&fadc_over30flag==false&&fadc_under10flag==false){
                pMax_cut->Fill(max_fadc);
                pInt_cut->Fill(sum_fadc);
                pMax_Int_cut->Fill(max_fadc,sum_fadc);
                pre_mean_cut[ch] += sum_fadc;	
                pre_max_cut[ch] += max_fadc;	
                pre_count_cut[ch]++;
                for( int clk = 2; clk < 4096; ++clk ) {
                    pHistcut->Fill( clk, fadcVar[ch][clk] );
                }
            }
            
            //=============
            //cut hist end
            //=============
        }
    }

    TCanvas cvsh( "cvsh", "cvsh", 1200, 600 );
    TCanvas cvsm( "cvsm", "cvsm", 1200, 600 );
    TCanvas cvsmin( "cvsmin", "cvsmin", 1200, 600 );
    TCanvas cvsi( "cvsi", "cvsi", 1200, 600 );
    TCanvas cvsmi( "cvsmi", "cvsmi", 1200, 600 );
    TCanvas cvsh_c( "cvsh_c", "cvsh_c", 1200, 600 );
    TCanvas cvsm_c( "cvsm_c", "cvsm_c", 1200, 600 );
    TCanvas cvsi_c( "cvsi_c", "cvsi_c", 1200, 600 );
    TCanvas cvsmi_c( "cvsmi_c", "cvsmi_c", 1200, 600 );
    
    //same draw
    TCanvas cvsi_same( "cvsi_same", "cvsi_same", 1200, 600 );

    const Int_t NRGBs = 5; const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );

    cvsh.Divide( 4, 2 );
    cvsm.Divide( 4, 2 );
    cvsmin.Divide( 4, 2 );
    cvsi.Divide( 4, 2 );
    cvsmi.Divide( 4, 2 );
    cvsh_c.Divide( 4, 2 );
    cvsm_c.Divide( 4, 2 );
    cvsi_c.Divide( 4, 2 );
    cvsmi_c.Divide( 4, 2 );

    //same draw
    cvsi_same.Divide( 4, 2 );

    for( int ch = 0; ch < 8; ++ch ) {
        cvsh.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH2F* pHist = histArr.at( ch );
        if( pHist == nullptr ) continue;

        pHist->GetXaxis()->SetTitle( "clock [5 MHz]" );
        //pHist->GetXaxis()->SetRangeUser( 1400.0, 1700.0 );
        //pHist->GetXaxis()->SetRangeUser( 0.0, 2048.0 );
        pHist->GetXaxis()->SetRangeUser( 480.0, 520.0 );
        pHist->GetYaxis()->SetTitle( "ADC count" );
        //pHist->GetYaxis()->SetRangeUser( -20, 200.0 );
        //pHist->GetYaxis()->SetRangeUser( -500, 500.0 );
        pHist->GetYaxis()->SetRangeUser( -100, 100.0 );
        pHist->GetZaxis()->SetTitle( "Entries" );
        pHist->GetZaxis()->SetRangeUser(0,totEvt/10);
        pHist->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsh_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH2F* pHistcut = histArrcut.at( ch );
        if( pHistcut == nullptr ) continue;

        pHistcut->GetXaxis()->SetTitle( "clock [5 MHz]" );
        pHistcut->GetXaxis()->SetRangeUser( 1400.0, 1700.0 );
        pHistcut->GetYaxis()->SetTitle( "ADC count" );
        pHistcut->GetYaxis()->SetRangeUser( -20, 200.0 );
        pHistcut->GetZaxis()->SetTitle( "Entries" );
        pHistcut->GetZaxis()->SetRangeUser(0,totEvt/10);
        pHistcut->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsm.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pMax = maxArr.at( ch );
        if( pMax == nullptr ) continue;

        pMax->GetXaxis()->SetTitle( "ADC count" );
        pMax->GetXaxis()->SetRangeUser( pre_max[ch]/pre_count[ch] - 50.0, pre_max[ch]/pre_count[ch] + 50.0 );
        pMax->GetYaxis()->SetTitle( "entry" );
        //pMax->GetYaxis()->SetRangeUser( 0, totEvt );
	int y_max;
        y_max = gPad->GetUymax();
        pMax->GetYaxis()->SetRangeUser( 1, y_max );
        pMax->Draw();
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsmin.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pMin = minArr.at( ch );
        if( pMin == nullptr ) continue;

        pMin->GetXaxis()->SetTitle( "ADC count" );
        pMin->GetXaxis()->SetRangeUser( pre_min[ch]/pre_count[ch] - 50.0, pre_min[ch]/pre_count[ch] + 50.0 );
        pMin->GetYaxis()->SetTitle( "entry" );
        pMin->GetYaxis()->SetRangeUser( 0, totEvt );
        pMin->Draw();
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsm_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pMaxcut = maxArrcut.at( ch );
        if( pMaxcut == nullptr ) continue;

        pMaxcut->GetXaxis()->SetTitle( "ADC count" );
        //pMaxcut->GetXaxis()->SetRangeUser( pre_max_cut[ch]/pre_count_cut[ch] - 50.0, pre_max_cut[ch]/pre_count_cut[ch] + 50.0 );
        pMaxcut->GetXaxis()->SetRangeUser( pre_max[ch]/pre_count[ch] - 50.0, pre_max[ch]/pre_count[ch] + 50.0 );
        pMaxcut->GetYaxis()->SetTitle( "entry" );
        pMaxcut->GetYaxis()->SetRangeUser( 0, totEvt );
        pMaxcut->Draw();
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsi.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pInt = intArr.at( ch );
        if( pInt == nullptr ) continue;

        pInt->GetXaxis()->SetTitle( "ADC sum" );
        pInt->GetXaxis()->SetRangeUser( -20000.0, 40000.0 );
        pInt->GetYaxis()->SetTitle( "entry" );
        pInt->Draw();
        TF1 *f1 = new TF1("f1","[0]*exp(-0.5*((x-[1])/[2])^2)",-10000,20000);
        f1->SetParameters(1,pre_mean[ch]/pre_count[ch],100);
        f1->SetLineColor(kRed);
        pInt->Fit(f1);
        double_t mean = f1->GetParameter(1);
        double_t hight = f1->GetParameter(0);
        cout << "ch:" << ch << "\tmean:" << mean <<endl;
        pInt->GetYaxis()->SetRangeUser( 0, hight*2 );
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsi_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pIntcut = intArrcut.at( ch );
        if( pIntcut == nullptr ) continue;

        pIntcut->GetXaxis()->SetTitle( "ADC sum" );
        pIntcut->GetXaxis()->SetRangeUser( -20000.0, 40000.0 );
        pIntcut->GetYaxis()->SetTitle( "entry" );
        pIntcut->Draw();
        TF1 *f1_c = new TF1("f1_c","[0]*exp(-0.5*((x-[1])/[2])^2)",-10000,20000);
        //f1_c->SetParameters(1,pre_mean_cut[ch]/pre_count_cut[ch],100);
        f1_c->SetParameters(1,pre_mean[ch]/pre_count[ch],100);
        f1_c->SetLineColor(kRed);
        pIntcut->Fit(f1_c);
        double_t mean_cut = f1_c->GetParameter(1);
        double_t hight_cut = f1_c->GetParameter(0);
        cout << "ch:" << ch << "\tmean_cut:" << mean_cut <<endl;
        pIntcut->GetYaxis()->SetRangeUser( 0, hight_cut*2 );
    }

    for( int ch = 0; ch < 8; ++ch ) {
        cvsmi.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH2F* pMax_Int = max_intArr.at( ch );
        if( pMax_Int == nullptr ) continue;

        pMax_Int->GetXaxis()->SetTitle( "ADC count" );
        pMax_Int->GetXaxis()->SetRangeUser( pre_max[ch]/pre_count[ch] - 50.0, pre_max[ch]/pre_count[ch] + 50.0 );
        pMax_Int->GetYaxis()->SetTitle( "ADC sum" );
        pMax_Int->GetYaxis()->SetRangeUser( -20000.0, 40000.0 );
        pMax_Int->GetYaxis()->SetRangeUser( pre_mean[ch]/pre_count[ch] - 15000.0, pre_max[ch]/pre_count[ch] + 15000.0 );
        pMax_Int->GetZaxis()->SetTitle( "Entries" );
        pMax_Int->GetZaxis()->SetRangeUser(0,totEvt/10);
        pMax_Int->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }
    for( int ch = 0; ch < 8; ++ch ) {
        cvsmi_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH2F* pMax_Intcut = max_intArrcut.at( ch );
        if( pMax_Intcut == nullptr ) continue;

        pMax_Intcut->GetXaxis()->SetTitle( "ADC count" );
        //pMax_Intcut->GetXaxis()->SetRangeUser( pre_max_cut[ch]/pre_count_cut[ch] - 50.0, pre_max_cut[ch]/pre_count_cut[ch] + 50.0 );
        pMax_Intcut->GetXaxis()->SetRangeUser( pre_max[ch]/pre_count[ch] - 50.0, pre_max[ch]/pre_count[ch] + 50.0 );
        pMax_Intcut->GetYaxis()->SetTitle( "ADC sum" );
        pMax_Intcut->GetYaxis()->SetRangeUser( -20000.0, 40000.0 );
        //pMax_Intcut->GetYaxis()->SetRangeUser( pre_mean_cut[ch]/pre_count_cut[ch] - 15000.0, pre_max_cut[ch]/pre_count_cut[ch] + 15000.0 );
        pMax_Intcut->GetYaxis()->SetRangeUser( pre_mean[ch]/pre_count[ch] - 15000.0, pre_max[ch]/pre_count[ch] + 15000.0 );
        pMax_Intcut->GetZaxis()->SetTitle( "Entries" );
        pMax_Intcut->GetZaxis()->SetRangeUser(0,totEvt/10);
        pMax_Intcut->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }
   
    //-----------
    //draw same
    //-----------
    for( int ch = 0; ch < 8; ++ch ) {
        cvsi_same.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pInt = intArr.at( ch );
        TH1F* pInt_c = intArrcut.at( ch );
        TH1F* pInt_BG = new TH1F("pInt_BG","pInt_BG",300,-20000.0,40000.0);
        if( pInt == nullptr ) continue;
        if( pInt_c == nullptr ) continue;
        pInt_BG->Add(pInt,1);
        pInt_BG->Add(pInt_c,-1);

        pInt->GetXaxis()->SetTitle( "ADC sum" );
        pInt->GetXaxis()->SetRangeUser( -20000.0, 40000.0 );
        pInt->GetYaxis()->SetTitle( "entry" );
        pInt->SetLineColor(kRed);
        pInt_c->SetLineColor(kBlue);
        pInt_BG->SetLineColor(kGreen);
        pInt->Draw();
        pInt_c->Draw("same");
        pInt_BG->Draw("same");
    }

    cvsh.SaveAs("wave.root");
    cvsh.SaveAs("wave.png");
    cvsm.SaveAs("max.root");
    cvsm.SaveAs("max.png");
    cvsmin.SaveAs("min.root");
    cvsmin.SaveAs("min.png");
    cvsi.SaveAs("int.root");
    cvsi.SaveAs("int.png");
    cvsmi.SaveAs("max_int.root");
    cvsmi.SaveAs("max_int.png");
    cvsh_c.SaveAs("wave_c.root");
    cvsh_c.SaveAs("wave_c.png");
    cvsm_c.SaveAs("max_c.root");
    cvsm_c.SaveAs("max_c.png");
    cvsi_c.SaveAs("int_c.root");
    cvsi_c.SaveAs("int_c.png");
    cvsmi_c.SaveAs("max_int_c.root");
    cvsmi_c.SaveAs("max_int_c.png");

    cvsi_same.SaveAs("int_same.root");
    cvsi_same.SaveAs("int_same.png");

    return;
}
