#include "inc/shinclude.h"

void dumpAllWaveformsRoot3( const String& inputFile, int ch_num)
//void dumpAllWaveformsRoot3( const String& inputFile)
{
    SetShStyle( );

    std::vector< TH2F* > histArr;
    histArr.reserve( 16 );
    for( int i = 0; i < 16; ++i ) {
        String histName = Form( "hist_%d", i );
        //histArr.push_back( new TH2F( histName.c_str( ), histName.c_str( ), 4096, 0.0, 4096.0, 4096, 0.0, 4096.0 ) );
        histArr.push_back( new TH2F( histName.c_str( ), histName.c_str( ), 4096, 0.0, 4096.0, 4096, -2096.0, 2000.0 ) );
    }

    TFile file( inputFile.c_str( ) );
    TTree* pTree = dynamic_cast< TTree* >( file.Get( "tree" ) );
    if( pTree == nullptr ) return;

    int   fadcVar[16][4096] = {};
    pTree->SetBranchAddress( "fadc0", fadcVar[0] );
    pTree->SetBranchAddress( "fadc1", fadcVar[1] );
    pTree->SetBranchAddress( "fadc2", fadcVar[2] );
    pTree->SetBranchAddress( "fadc3", fadcVar[3] );
    pTree->SetBranchAddress( "fadc4", fadcVar[4] );
    pTree->SetBranchAddress( "fadc5", fadcVar[5] );
    pTree->SetBranchAddress( "fadc6", fadcVar[6] );
    pTree->SetBranchAddress( "fadc7", fadcVar[7] );

    //ofuji
    TH1D *h1 = new TH1D("h1","h1",1000,0,4000);
    TH1D *h2 = new TH1D("h2","h2",3000,-100000,200000);
    TH1D *h2_num = new TH1D("h2_num","h2_num",3000,-100000,200000);
    TH2D *h3 = new TH2D("h3","h3",1000,0,4000,3000,-100000,200000);
    TH1D *h4 = new TH1D("h4","h4",1000,0,4000);
    TH1D *h5 = new TH1D("h5","h5",3000,-100000,200000);
    TH2D *h6 = new TH2D("h6","h6",1000,0,4000,3000,-100000,200000);
    TH2D *hwave = new TH2D("hwave","hwave",4096,0,4096,4096,-2096,2000);
    TH2D *hwave_num = new TH2D("hwave_num","hwave_num",4096,0,4096,4096,-2096,2000);
    TH1D *hint = new TH1D("hint","hint",3000,-100000,200000);
    int pre_mean = 0;
    int pre_count = 0;
    int sum_fadc_count = 0;
    int ev_num = 10;

    int totEvt = pTree->GetEntries( );
    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );
        //pTree->GetEntry( ev_num );
        //ShUtil::PrintProgressBar( ev_num, totEvt );
        
        //for( int ch = 0; ch < 16; ++ch ) {
        //for( int ch = 0; ch < 8; ++ch ) {
        for( int ch = ch_num; ch < ch_num+1; ++ch ) {
            TH2F* pHist = histArr.at( ch );
            if( pHist == nullptr ) continue;
       	    double base_fadc = 0.;
       	    double max_fadc = 0.;
       	    double sum_fadc = 0.;
       	    double latter_max_fadc = 0.;
	    int max_clock = 0;
            for( int clk = 2; clk < 4096; ++clk ) {
   	    	if(fadcVar[ch][clk]>2000){
		     fadcVar[ch][clk]-=4096;
	    	}
		if(clk < 200){
			base_fadc += fadcVar[ch][clk];
		}
	    }
       	    base_fadc = base_fadc/198;
            for( int clk = 2; clk < 4096; ++clk ) {
		sum_fadc += fadcVar[ch][clk] - base_fadc;
                if(max_fadc<=fadcVar[ch][clk]){
			max_fadc = fadcVar[ch][clk]; 
			max_clock = clk;
		}
		if((clk>=1400)&&(latter_max_fadc<=fadcVar[ch][clk])){
			latter_max_fadc = fadcVar[ch][clk];
		}
				
            }
	    //if((latter_max_fadc<=100)&&(fadcVar[8][200]<=50)&&(fadcVar[8][300]<=50)&&(max_fadc<=1500)&&(max_clock>=650)&&(max_clock<=1200)){
	    //if((latter_max_fadc<=100)&&(fadcVar[8][200]<=50)&&(fadcVar[8][300]<=50)&&(max_fadc<=1500)){//testpuls
	    h1->Fill(max_fadc);
	    h2->Fill(sum_fadc);
	    if(evt == ev_num){
	    	h2_num->Fill(sum_fadc);
	    }
	    if(evt%500==0){
	    	cout << "now eve%500 = 0 ; base:" << base_fadc << " sum:" << sum_fadc <<endl;
	    }
		sum_fadc_count++;
		//cout << "h2->Fill:" << sum_fadc_count << " ; " << sum_fadc/1000 << "k" << endl;
		h3->Fill(max_fadc,sum_fadc);
		pre_mean += sum_fadc;	
		pre_count++;
		if(sum_fadc<=(max_fadc*444.4-37777.8)){
			h4->Fill(max_fadc);
			h5->Fill(sum_fadc);
			h6->Fill(max_fadc,sum_fadc);
		}
            	for( int clk = 0; clk < 4096; ++clk ) {
               	 pHist->Fill( clk, fadcVar[ch][clk] );
		 //if((-15000<=sum_fadc)&&(sum_fadc<=-8000)){//cut
		 if((-1000<=sum_fadc)&&(sum_fadc<=1000)){//cut
               	 	hwave->Fill( clk, fadcVar[ch][clk] );
			hint->Fill(sum_fadc);
		}
		if(evt == ev_num){
               	 	hwave_num->Fill( clk, fadcVar[ch][clk] );
		}

            	}
            //}
        }
    }

    TCanvas cvs( "cvs", "cvs", 1200, 600 );

    const Int_t NRGBs = 5; const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );

    //cvs.Divide( 4, 4 );
    //cvs.Divide( 4, 2 );

    TCanvas *cwave = new TCanvas("cwave","cwave");
    TCanvas *cwave_num = new TCanvas("cwave_num","cwave_num");
    //for( int ch = 0; ch < 16; ++ch ) {
    //for( int ch = 0; ch < 8; ++ch ) {
    for( int ch = ch_num; ch < ch_num+1; ++ch ) {
        //cvs.cd( ch+1 );
	cwave->cd();
        gPad->SetRightMargin( 0.2 );
        TH2F* pHist = histArr.at( ch );
        if( pHist == nullptr ) continue;

        pHist->GetXaxis()->SetTitle( "clock [5 MHz]" );
        //pHist->GetXaxis()->SetRangeUser( 0.0, 2056.0 );
        pHist->GetYaxis()->SetTitle( "ADC count" );
        //pHist->GetYaxis()->SetRangeUser( -2096, 2000.0 );
        pHist->GetZaxis()->SetTitle( "Entries" );
        pHist->GetZaxis()->SetRangeUser(0,totEvt/10);
        pHist->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }


    TF1 *f1 = new TF1("f1","[0]*exp(-0.5*((x-[1])/[2])^2)",0,20000000);
    //f1->SetParameters(1,100000,100);
    //f1->SetParameters(1,2000,100);
    f1->SetParameters(1,pre_mean/pre_count,100);

    TCanvas *c1 = new TCanvas("c1","c1");
    TCanvas *c2 = new TCanvas("c2","c2");
    TCanvas *c3 = new TCanvas("c3","c3");
    TCanvas *c4 = new TCanvas("c4","c4");
    TCanvas *c5 = new TCanvas("c5","c5");
    TCanvas *c6 = new TCanvas("c6","c6");
    TCanvas *cpeak_wave = new TCanvas("cpeak_wave","cpeak_wave");
    TCanvas *cpeak_int = new TCanvas("cpeak_int","cpeak_int");
    TCanvas *c2_num = new TCanvas("c2_num","c2_num");
    c1->cd();
    h1->Draw();
    c1->SaveAs("hight.pdf");
    c1->SaveAs("hight.root");
    c2->cd();
    h2->Draw();
    //h2->Fit("f1","","",2000,10000);
    //h2->Fit("f1");
    double_t mean = f1->GetParameter(1);
    cout << "mean:" << mean <<endl;
    h2->SaveAs("int_hist.root");
    c2->SaveAs("int.pdf");
    c2->SaveAs("int.root");
    c2_num->cd();
    h2_num->Draw();
    h2_num->SaveAs("int_num.root");
    c3->cd();
    h3->Draw("colz");
    c3->SaveAs("hight-int.pdf");
    c3->SaveAs("hight-int.root");
    c4->cd();
    h4->Draw();
    c4->SaveAs("hight_cut.pdf");
    c4->SaveAs("hight_cut.root");
    c5->cd();
    h5->Draw();
    c5->SaveAs("int_cut.pdf");
    c5->SaveAs("int_cut.root");
    c6->cd();
    h6->Draw("colz");
    c6->SaveAs("hight-int_cut.pdf");
    c6->SaveAs("hight-int_cut.root");
    cwave_num->cd();
    hwave_num->Draw("colz");
    cwave_num->SaveAs("wave_num.root");
    cpeak_wave->cd();
    hwave->Draw("colz");
    cpeak_wave->SaveAs("peak_wave.pdf");
    cpeak_wave->SaveAs("peak_wave.root");
    cpeak_int->cd();
    hint->Draw();
    cpeak_int->SaveAs("peak_int.pdf");
    cpeak_int->SaveAs("peak_int.root");
    cwave->SaveAs("wavef.pdf");
    cwave->SaveAs("wavef.root");
    
    return;

}
