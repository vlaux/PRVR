/*
 * Utils.h
 *
 *  Created on: 5 de dez de 2016
 *  Author: leo
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "randutils.h"

#include <string>
#include <vector>
#include <random>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <cmath>

typedef std::vector<std::vector<int>> Matriz;
typedef std::tuple<int, int, int> Movimento;

namespace utils{

    namespace functional {
        template <typename Collection,typename Condition>
        inline bool exists(Collection const & col,Condition con) {
            auto exist = std::find_if(col.begin(),col.end(),con);
            return exist != col.end();
        }

        template <typename Collection,typename Predicate>
        inline Collection filterNot(Collection const & col,Predicate predicate ) {
            auto returnIterator = std::remove_if(col.begin(),col.end(),predicate);
            col.erase(returnIterator,std::end(col));
            return col;
        }

        //--------------------------------------------------V

        template <typename Collection,typename unop>
        inline void for_each(Collection const & col, unop op){
            std::for_each(col.begin(),col.end(),op);
        }

        template <typename Collection,typename binop>
        inline Collection zip(Collection const & fc,Collection sc,binop op) {
            Collection zipped(fc.size());
            std::transform(fc.begin(),fc.end(),sc.begin(),zipped.begin(),op);
            return zipped;
        }

        template <class Iter, typename result, typename unop>
        inline result reduce(Iter start, Iter end, result value, unop op){
            return std::accumulate(start, end, value, op);
        }

        template <typename Collection, typename result, typename unop>
        inline result reduce(Collection const & col, result value, unop op){
            return std::accumulate(col.begin(), col.end(), value, op);
        }

        template <typename C,typename unop>
        inline C map(C const & container,unop op) {
            C mapped(container.size());
            std::transform(container.begin(),container.end(),mapped.begin(),op);
            return mapped;
        }

        template<typename C, typename P>
        inline C filter(C const & container, P pred){
            C filtered(container.size());
            auto it = copy_if(container.begin(), container.end(), filtered.begin(), pred);
            filtered.resize(std::distance(filtered.begin(), it));
            return filtered;
        }
    }

        namespace lists {
        template <typename Iter, typename result>
        inline result sum(Iter start, Iter end, result initialValue){
            return utils::functional::reduce(start, end, initialValue, [](result a, result b){return a + b;});
        }

        template <typename Collection, typename result>
        inline result sum(Collection const & col, result initialValue){
            return utils::functional::reduce(col, initialValue, [](result a, result b){return a + b;});
        }

        template <typename Iter, typename result>
        inline result prod(Iter start, Iter end, result initialValue){
            return utils::functional::reduce(start, end, initialValue, [](result a, result b){return a * b;});
        }

        template <typename Collection, typename result>
        inline result prod(Collection const & col, result initialValue){
            return utils::functional::reduce(col, initialValue, [](result a, result b){return a * b;});
        }

        template <typename Collection, typename valType>
        inline void values(Collection& col, valType value){
            for (auto &e: col){
                e = value;
            }
        }

        template <typename Collection>
        inline void ones(Collection& col){
            utils::lists::values(col, 1.0);
        }

        template <typename Collection>
        inline void zeros(Collection& col){
            utils::lists::values(col, 0.0);
        }

        template<class T1, class T2, class T3>
        inline void printVet(const T1* vet, T2 ini, T3 fim){
            for (size_t i = ini; i < (size_t) fim; i++)
            {
                std::cout << i << " - " << vet[i] << std::endl;
            }
        }

        template<class T1, class T2, class T3, class A>
        inline void printVet(const T1* vet, T2 ini, T3 fim, A access){
            for (size_t i = ini; i < (size_t) fim; i++)
            {
                std::cout << i << " - " << access(vet[i]) << std::endl;
            }
        }
    }

    namespace random{
        class mt19937_rng_singleton
        {
        public:
            static utils::random::mt19937_rng_singleton* getInstance();

            randutils::mt19937_rng& generator(){
                return rng;
            }

            void reSeed(){
                rng.seed();
            }

        protected:
            mt19937_rng_singleton();
        private:
            static utils::random::mt19937_rng_singleton* _instance;

            randutils::mt19937_rng rng;
        };

        template<class RandomGenerator>
        inline double rand_0_1(RandomGenerator& g) {
            std::uniform_real_distribution<double> dis(0.0, 1.0);
            return dis(g);
        }

        inline double rand_0_1() {
            randutils::mt19937_rng& rng = utils::random::mt19937_rng_singleton::getInstance()->generator();
            return rng.uniform(0.0, 1.0);
        }

        template<class Iter, class RandomGenerator>
        inline Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
            std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
            std::advance(start, dis(g));
            return start;
        }

        template<class Iter>
        inline Iter choose(Iter start, Iter end) {
            randutils::mt19937_rng& rng = utils::random::mt19937_rng_singleton::getInstance()->generator();
            return rng.choose(start, end);
        }

        template<class Con>
        inline auto pick(Con con) -> decltype(*std::begin(con)) {
            randutils::mt19937_rng& rng = utils::random::mt19937_rng_singleton::getInstance()->generator();
            return rng.pick(con);
        }

        template<class Iter>
        inline void shuffle(Iter start, Iter end) {
            randutils::mt19937_rng& rng = utils::random::mt19937_rng_singleton::getInstance()->generator();
            rng.shuffle(start, end);
        }

        template<class Con>
        inline void shuffle(Con con) {
            randutils::mt19937_rng& rng = utils::random::mt19937_rng_singleton::getInstance()->generator();
            rng.shuffle(con);
        }

        template <typename ColProb>
        inline int select_index_randomly(ColProb prob) {
            randutils::mt19937_rng& rng = utils::random::mt19937_rng_singleton::getInstance()->generator();
            std::discrete_distribution<int> dist(prob.begin(), prob.end());
            return dist(rng.engine());
        }
    }

    // ------------------------------------------------- //

    template<class T1, class T2, class T3, class T4>
    inline double calcula_desvio(const T1* vet, T2 ini, T3 fim, T4 media){
        double sum = 0.0;
        for (size_t i = ini; i < (size_t) fim; i++)
        {
            sum += pow((vet[i] - media), 2.0);
        }

        sum = sum / (fim - ini);
        sum = sqrt(sum);

        return sum;
    }

    template<class T1, class T2, class T3>
    inline double calcula_media(const T1* vet, T2 ini, T3 fim){
        double sum = 0.0;
        for (size_t i = ini; i < (size_t) fim; i++)
        {
            sum += vet[i];
        }
        return sum / (fim - ini);
    }

    template<class T1, class T2, class T3, class T4>
    inline double calcula_desvio(double (*f)(T1), const T1* vet, T2 ini, T3 fim, T4 media){
        double sum = 0.0;
        for (size_t i = ini; i < (size_t) fim; i++)
        {
            sum += pow(((*f)(vet[i]) - media), 2.0);
        }

        sum = sum / (fim - ini);
        sum = sqrt(sum);

        return sum;
    }

    template<class T1, class T2, class T3>
    inline double calcula_media(double (*f)(T1), const T1* vet, T2 ini, T3 fim){
        double sum = 0.0;
        for (size_t i = ini; i < (size_t) fim; i++)
        {
            sum += (*f)(vet[i]);
        }
        return sum / (fim - ini);
    }

    template<class T1, class T2>
    inline double media_harmonica(T1 num1, T2 num2){
        return 2 * ((num1 * num2) / (num1 + num2));
    }
}

#endif /* UTILS_H_ */
