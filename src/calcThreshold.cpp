#include <Rcpp.h>
#include <vector>
#include <algorithm>

using namespace Rcpp;
using namespace std;

// [[Rcpp::export(.calcThreshold)]]
double calcThreshold(const NumericVector& ye, const NumericVector& yt, bool Gm) {
    vector< pair<double,bool> > y;
    y.reserve(yt.size());

    double npos=0, nneg=0;
    unsigned int i;
    for(i=0; i<yt.size(); i++) {
        if(yt[i] != 0) {
            if(yt[i]>0) {
                y.push_back(pair<double,bool>(ye[i], true));
                npos++;
            }
            else {
                y.push_back(pair<double,bool>(ye[i], false));
                nneg++;
            }
        }
    }
    sort(y.begin(), y.end());

    double obj, best=0, TP=npos, TN=0;
    int lbest=-1, rbest=-1;
    if(Gm) {
        for(i=0; i<y.size()-1; i++) {
            if(y[i].second) TP--;
            else TN++;
            obj = TP / npos * TN / nneg; // Gm
            if(obj > best) {
                lbest=i; rbest=i+1;
                best = obj;
            }
            else if (obj == best)
                rbest=i;
        }
    } else {
        for(i=0; i<y.size()-1; i++) {
            if(y[i].second) TP--;
            else TN++;
            obj = TP / npos * TP / (y.size()-i-1); // G
            if(obj > best) {
                lbest=i; rbest=i+1;
                best = obj;
            }
            else if(obj == best)
                rbest=i;
        }
    }

    return 0.5 * y[lbest].first + 0.5 * y[rbest].first;
}

