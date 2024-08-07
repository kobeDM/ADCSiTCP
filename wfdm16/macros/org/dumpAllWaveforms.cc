#include "inc/shinclude.h"

void dumpAllWaveforms( const String& inputFile,const String& outputFile)
{
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    std::vector< TH2F* > histArr;
    std::vector< TH1F* > maxArr;
    std::vector< TH1F* > minArr;
    std::vector< TH1F* > intArr;
    std::vector< TH2F* > max_intArr;
    std::vector< TH2F* > histArrcut;
    std::vector< TH1F* > maxArrcut;
    std::vector< TH1F* > intArrcut;
    std::vector< TH2F* > max_intArrcut;
    histArr.reserve( chN );
    maxArr.reserve( chN );
    minArr.reserve( chN );
    intArr.reserve( chN );
    max_intArr.reserve( chN );
    histArrcut.reserve( chN );
    maxArrcut.reserve( chN );
    intArrcut.reserve( chN );
    max_intArrcut.reserve( chN );

    for( int i = 0; i < chN; ++i ) {
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

    //==============
    // Fill
    //==============

    TFile file( inputFile.c_str( ) );
    TTree* pTree = dynamic_cast< TTree* >( file.Get( "tree" ) );
    if( pTree == nullptr ) return;

    int fadcVar[chN][clkN] = {};
    for (int i = 0; i < chN; i++) 
        pTree->SetBranchAddress( Form("fadc%d", i), fadcVar[i]);

    vector<int > pre_mean( chN );
    vector<int > pre_max( chN );
    vector<int > pre_min( chN );
    vector<int > pre_count( chN );
    vector<int > pre_mean_cut( chN );
    vector<int > pre_max_cut( chN );
    vector<int > pre_count_cut( chN );

    int totEvt = pTree->GetEntries( );
    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );

        for( int ch = 0; ch < chN; ++ch ) {
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
            double max_fadc = -4096.;
            double min_fadc = 4096.;
            double sum_fadc = 0.;
            int max_clock = 0;
            int min_clock = 0;
            int clk_offset = 2;
            int clk_base_len = 200;

            for( int clk = clk_offset; clk < clk_base_len; ++clk )
                base_fadc += fadcVar[ch][clk];
            base_fadc = base_fadc / ( clk_base_len - clk_offset );

            for( int clk = clk_offset; clk < chN; ++clk ) {
                sum_fadc += fadcVar[ch][clk] - base_fadc;
                fadcVar[ch][clk] -= base_fadc;
                if( max_fadc < fadcVar[ch][clk] ) {
                    max_fadc = fadcVar[ch][clk]; 
                    max_clock = clk;
                }
                if( min_fadc > fadcVar[ch][clk] ) {
                    min_fadc = fadcVar[ch][clk]; 
                    min_clock = clk;
		        }
            }
            
            pMax->Fill( max_fadc );
            pMin->Fill( min_fadc );
            pInt->Fill( sum_fadc );
            pMax_Int->Fill( max_fadc, sum_fadc );
            pre_mean[ch] += sum_fadc;
            pre_max[ch] += max_fadc;
            pre_count[ch]++;
            for( int clk = clk_offset; clk < clkN; ++clk )
                pHist->Fill( clk, fadcVar[ch][clk] );
            
            pMax_cut->Fill( max_fadc );
            pInt_cut->Fill( sum_fadc );
            pMax_Int_cut->Fill( max_fadc, sum_fadc );
            pre_mean_cut[ch] += sum_fadc;
            pre_max_cut[ch] += max_fadc;
            pre_count_cut[ch]++;
            for( int clk = clk_offset; clk < clkN; ++clk ) {
                pHistcut->Fill( clk, fadcVar[ch][clk] );
            }
        }
    }

    //==============
    // Plot
    //==============

    const int cvs_x = 1200;
    const int cvs_y = 600;

    TCanvas cvsh( "cvsh", "cvsh", cvs_x, cvs_y );
    TCanvas cvsm( "cvsm", "cvsm", cvs_x, cvs_y );
    TCanvas cvsmin( "cvsmin", "cvsmin", cvs_x, cvs_y );
    TCanvas cvsi( "cvsi", "cvsi", cvs_x, cvs_y );
    TCanvas cvsmi( "cvsmi", "cvsmi", cvs_x, cvs_y );
    TCanvas cvsh_c( "cvsh_c", "cvsh_c", cvs_x, cvs_y );
    TCanvas cvsm_c( "cvsm_c", "cvsm_c", cvs_x, cvs_y );
    TCanvas cvsi_c( "cvsi_c", "cvsi_c", cvs_x, cvs_y );
    TCanvas cvsmi_c( "cvsmi_c", "cvsmi_c", cvs_x, cvs_y );
    TCanvas cvsmax_min( "cvsmax_min", "cvsmax_min", cvs_x, cvs_y );
    
    TCanvas cvsi_same( "cvsi_same", "cvsi_same", cvs_x, cvs_y );

    const Int_t NRGBs = 5; const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );

    const int div_x = 4;
    const int div_y = 2;

    cvsh.Divide( div_x, div_y );
    cvsm.Divide( div_x, div_y );
    cvsmin.Divide( div_x, div_y );
    cvsi.Divide(div_x, div_y );
    cvsmi.Divide( div_x, div_y );
    cvsh_c.Divide( div_x, div_y );
    cvsm_c.Divide( div_x, div_y );
    cvsi_c.Divide( div_x, div_y );
    cvsmi_c.Divide( div_x, div_y );
    cvsmax_min.Divide( div_x, div_y );

    cvsi_same.Divide( div_x, div_y );

    String file_name = outputFile + "/stat.dat";
    ofstream fout;
    fout.open(file_name);
    if( !fout ) cout << "*cant open output txt file*" <<endl;

    const double wf_x_min = 480.0, wf_x_max = 520.0;
    const double wf_y_min = -500.0, wf_y_max = 500.0;
    const double wfc_x_min = 480.0, wfc_x_max = 520.0;
    const double wfc_y_min = -500.0, wfc_y_max = 500.0;
    const double peak_min = 0.0, peak_max = 500.0;

    for( int ch = 0; ch < chN; ++ch ) {
        //===================
        //  Waveform hist
        //===================
        
        cvsh.cd( ch+1 );
        // cvsh.cd( ch+1 )->SetLogz();
        gPad->SetRightMargin( 0.2 );
        TH2F* pHist = histArr.at( ch );
        if( pHist == nullptr ) continue;

        pHist->GetXaxis()->SetTitle( "clock [40 MHz]" );
        pHist->GetYaxis()->SetTitle( "ADC count" );
        pHist->GetXaxis()->SetRangeUser( wf_x_min, wf_x_max );
        pHist->GetYaxis()->SetRangeUser( wf_y_min, wf_y_max );
        pHist->GetZaxis()->SetTitle( "Entries" );
        pHist->GetZaxis()->SetRangeUser(0, totEvt);
        pHist->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    for( int ch = 0; ch < chN; ++ch ) {
        //====================
        //  waveform hist_c
        //====================

        cvsh_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH2F* pHistcut = histArrcut.at( ch );
        if( pHistcut == nullptr ) continue;

        pHistcut->GetXaxis()->SetTitle( "clock [5 MHz]" );
        pHistcut->GetXaxis()->SetRangeUser( wfc_x_min, wfc_x_max );
        pHistcut->GetYaxis()->SetTitle( "ADC count" );
        pHistcut->GetYaxis()->SetRangeUser( wfc_y_min, wfc_y_max);
        pHistcut->GetZaxis()->SetTitle( "Entries" );
        pHistcut->GetZaxis()->SetRangeUser( 0, totEvt );
        pHistcut->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    for( int ch = 0; ch < chN; ++ch ) {
        //===================
        //  peak hist
        //===================

        cvsm.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        gPad->SetLogy();
        gPad->SetGridy();
        TH1F* pMax = maxArr.at( ch );
        if( pMax == nullptr ) continue;

        pMax->GetXaxis()->SetTitle( "ADC count" );
        pMax->GetXaxis()->SetRangeUser( peak_min, peak_max );
        pMax->GetYaxis()->SetTitle( "entry" );
        pMax->Draw();
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        // int start = pMax->FindBin(0);
        // int end = pMax->FindBin(25);
        // int max_bin = 0;
        // int max_ent = 0; 
        // int ent = 0;
        // for( int i = start; i < end; i++ ) {
        //     ent = pMax->GetBinContent(i);
        //     if( max_ent <= ent) {
        //         max_bin = i;
        //         max_ent = ent;
        //     }
        // }
        // pMax->GetYaxis()->SetRangeUser( 1, max_ent*2 );
        // if(ch<=3){
        //     cvsmax_min.cd(ch+1);
        //     gPad->SetLogy();
        //     gPad->SetGridy();
        //     pMax->Draw();
        //     TF1 *f1 = new TF1("f1","gaus");
        //     f1->SetLineColor(kBlue);
        //     if(ch==0){
        //         pMax->Fit(f1,"","",40*0.625,80*0.625);
        //     }else{
        //         pMax->Fit(f1,"","",40,80);
        //     }
        //     double mean = f1->GetParameter(1);
        //     double mean_error = f1->GetParError(1);
        //     double sigma = f1->GetParameter(2);
        //     double sigma_error = f1->GetParError(2);
        //     fout << mean << "\t" << mean_error << "\t" << sigma << "\t" << sigma_error << "\t" << abs(sigma/mean) <<endl;
        // }

    }

    for( int ch = 0; ch < chN; ++ch ) {
        //===================
        //  min hist
        //===================
    
        cvsmin.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        gPad->SetLogy();
        gPad->SetGridy();
        TH1F* pMin = minArr.at( ch );
        if( pMin == nullptr ) continue;

        pMin->GetXaxis()->SetTitle( "ADC count" );
        pMin->GetXaxis()->SetRangeUser( peak_max * -1.0, peak_min * -1.0 );
        pMin->GetYaxis()->SetTitle( "entry" );
        pMin->Draw();
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        // int start = pMin->FindBin(-25);
        // int end = pMin->FindBin(25);
        // int max_bin = 0;
        // int max_ent = 0; 
        // int ent = 0;
        // for(int i=start;i<end;i++){
        //     ent = pMin->GetBinContent(i);
        //     if(max_ent<=ent){
        //         max_bin = i;
        //         max_ent = ent;
        //     }
        // }
        // pMin->GetYaxis()->SetRangeUser( 1, max_ent*2 );
        // if(ch>=4){
        //     cvsmax_min.cd(ch+1);
        //     gPad->SetLogy();
        //     gPad->SetGridy();
        //     pMin->Draw();
        //     TF1 *f1 = new TF1("f1","gaus");
        //     f1->SetLineColor(kRed);
        //     pMin->Fit(f1,"","",-80,-40);
        //     double mean = f1->GetParameter(1);
        //     double mean_error = f1->GetParError(1);
        //     double sigma = f1->GetParameter(2);
        //     double sigma_error = f1->GetParError(2);
        //     fout << mean << "\t" << mean_error << "\t" << sigma << "\t" << sigma_error << "\t" << abs(sigma/mean) <<endl;
        // }

    }

    for( int ch = 0; ch < chN; ++ch ) {
        cvsm_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pMaxcut = maxArrcut.at( ch );
        if( pMaxcut == nullptr ) continue;

        pMaxcut->GetXaxis()->SetTitle( "ADC count" );
        //pMaxcut->GetXaxis()->SetRangeUser( pre_max_cut[ch]/pre_count_cut[ch] - 50.0, pre_max_cut[ch]/pre_count_cut[ch] + 50.0 );
        pMaxcut->GetXaxis()->SetRangeUser( pre_max[ch] / pre_count[ch] - 50.0, pre_max[ch] / pre_count[ch] + 50.0 );
        pMaxcut->GetYaxis()->SetTitle( "entry" );
        pMaxcut->GetYaxis()->SetRangeUser( 0, totEvt );
        pMaxcut->Draw();
    }

    for( int ch = 0; ch < chN; ++ch ) {
        cvsi.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pInt = intArr.at( ch );
        if( pInt == nullptr ) continue;

        pInt->GetXaxis()->SetTitle( "ADC sum" );
        pInt->GetXaxis()->SetRangeUser( -20000.0, 40000.0 );
        pInt->GetYaxis()->SetTitle( "entry" );
        pInt->Draw();
        // TF1 *f1 = new TF1("f1","[0]*exp(-0.5*((x-[1])/[2])^2)",-10000,20000);
        // f1->SetParameters(1,pre_mean[ch]/pre_count[ch],100);
        // f1->SetLineColor(kRed);
        // pInt->Fit(f1);
        // double_t mean = f1->GetParameter(1);
        // double_t hight = f1->GetParameter(0);
        // cout << "ch:" << ch << "\tmean:" << mean <<endl;
        // pInt->GetYaxis()->SetRangeUser( 0, hight*2 );
    }

    for( int ch = 0; ch < chN; ++ch ) {
        cvsi_c.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pIntcut = intArrcut.at( ch );
        if( pIntcut == nullptr ) continue;

        pIntcut->GetXaxis()->SetTitle( "ADC sum" );
        pIntcut->GetXaxis()->SetRangeUser( -20000.0, 40000.0 );
        pIntcut->GetYaxis()->SetTitle( "entry" );
        pIntcut->Draw();
        // TF1 *f1_c = new TF1("f1_c","[0]*exp(-0.5*((x-[1])/[2])^2)",-10000,20000);
        // f1_c->SetParameters(1,pre_mean_cut[ch]/pre_count_cut[ch],100);
        // f1_c->SetParameters(1,pre_mean[ch]/pre_count[ch],100);
        // f1_c->SetLineColor(kRed);
        // pIntcut->Fit(f1_c);
        // double_t mean_cut = f1_c->GetParameter(1);
        // double_t hight_cut = f1_c->GetParameter(0);
        // cout << "ch:" << ch << "\tmean_cut:" << mean_cut <<endl;
        // pIntcut->GetYaxis()->SetRangeUser( 0, hight_cut*2 );
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
   
    for( int ch = 0; ch < 8; ++ch ) {
        cvsi_same.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH1F* pInt = intArr.at( ch );
        TH1F* pInt_c = intArrcut.at( ch );
        TH1F* pInt_BG = new TH1F(Form("pInt_BG_%d", ch),Form("pInt_BG_%d", ch),300,-20000.0,40000.0);
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

    cvsh.SaveAs(Form("%s/wave.png", outputFile.c_str()));
    cvsm.SaveAs(Form("%s/max.png", outputFile.c_str()));
    cvsmin.SaveAs(Form("%s/min.png", outputFile.c_str()));
    cvsi.SaveAs(Form("%s/int.png", outputFile.c_str()));
    cvsmi.SaveAs(Form("%s/max_int.png", outputFile.c_str()));
    cvsh_c.SaveAs(Form("%s/wave_c.png", outputFile.c_str()));
    cvsm_c.SaveAs(Form("%s/max_c.png", outputFile.c_str()));
    cvsi_c.SaveAs(Form("%s/int_c.png", outputFile.c_str()));
    cvsmi_c.SaveAs(Form("%s/max_int_c.png", outputFile.c_str()));
    cvsi_same.SaveAs(Form("%s/int_same.png", outputFile.c_str()));
    cvsmax_min.SaveAs(Form("%s/max_min_c.png", outputFile.c_str()));

    fout.close();

    return;
}