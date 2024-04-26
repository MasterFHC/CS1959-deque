#pragma GCC opitimize("Ofast")
#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {
template<class T> class Node{
public:
    T* info;
    Node* pre;
    Node* nxt;
    Node(): info(nullptr), nxt(nullptr), pre(nullptr) {}
    Node(Node* pre, Node* nxt) : info(nullptr), nxt(nxt), pre(pre) {}
    Node(T* info, Node* pre, Node* nxt) : info(info), nxt(nxt), pre(pre) {}
};
template<class T> class double_list{
public:
    class iterator;
	
    iterator head, tail;
    Node<T> *head_node, *tail_node;
// --------------------------
    inline void initialize(){
        head_node = new Node<T>(), tail_node = new Node<T>();
        head_node->nxt = tail_node, head_node->pre = nullptr;
        tail_node->pre = head_node, tail_node->nxt = nullptr;
        head.ptr = head_node, head.cont = this;
        tail.ptr = tail_node, tail.cont = this;
    }
	double_list(){
        initialize();
	}
	double_list(const double_list<T> &other){
        initialize();
        for(auto it=other.begin();it!=other.end();it++){
            insert_tail((*it));
        }
	}
    double_list<T> & operator=(const double_list<T> &other){
        if(&other == this) return (*this);
        while(!empty()){
            delete_head();
        }
        head_node->nxt = head_node->pre = tail_node->nxt = tail_node->pre = nullptr;
        delete head_node;
        delete tail_node;
        initialize();
        for(auto it=other.begin();it!=other.end();it++){
            insert_tail((*it));
        }
        return (*this);
	}
	~double_list(){
        // std::cout<<"destructing double_list!"<<std::endl;
        // std::cout<<head_node<<std::endl;
        // std::cout<<tail_node<<std::endl;
        // std::cout<<head_node->nxt<<std::endl;
        // std::cout<<head_node->nxt->nxt<<std::endl;
        while(!empty()){
            // std::cout<<"not empty!"<<std::endl;
            delete_head();
        }
        // std::cout<<"mid_check!"<<std::endl;
        head_node->nxt = head_node->pre = tail_node->nxt = tail_node->pre = nullptr;
        delete head_node;
        delete tail_node;
	}

	class iterator{
	public:
    	Node<T>* ptr;//pointing node
        double_list* cont;//container
    
	    // --------------------------
		iterator() : ptr(nullptr), cont(nullptr) {}
        iterator(Node<T>* t, double_list* cont) : ptr(t), cont(cont) {}
		iterator(const iterator &t) : ptr(t.ptr), cont(t.cont) {}
		~iterator(){
            ptr = nullptr;
            cont = nullptr;
        }
        /**
		 * iter++
		 */
		iterator operator++(int) {
            if(ptr == cont->get_tail()) throw "segmentation fault it++";
            iterator ret = *this;
            ptr = ptr->nxt;
            return ret;
		}
        /**
		 * ++iter
		 */
		iterator &operator++() {
            if(ptr == cont->get_tail()) throw "segmentation fault ++it";
            ptr = ptr->nxt;
            return (*this);
		}
        /**
		 * iter--
		 */
		iterator operator--(int) {
            if(ptr == cont->get_head()->nxt) throw "segmentation fault it--";
            iterator ret = *this;
            ptr = ptr->pre;
            return ret;
		}
        /**
		 * --iter
		 */
		iterator &operator--() {
            if(ptr == cont->get_head()->nxt) throw "segmentation fault --it";
            ptr = ptr->pre;
            return (*this);
		}
		T &operator*() const {
            if(ptr == nullptr or ptr == cont->get_head() or ptr == cont->get_tail()) throw "invalid when *it";
            return (*(ptr->info));
		}
        bool is_begin() const{
            return ptr == cont->get_head();
        }
        bool is_end() const{
            return ptr == cont->get_tail();
        }
        Node<T>* get() const{
            return ptr;
        }
        T getvalue() const{
            return *(ptr->info);
        }
		T *operator->() const noexcept {
            return (ptr->info);
		}
		bool operator==(const iterator &rhs) const {
            return (ptr == rhs.ptr);
    	}
		bool operator!=(const iterator &rhs) const {
            return (ptr != rhs.ptr);
		}
	};
    Node<T>* get_head() const{
        return head_node;
    }
    Node<T>* get_tail() const{
        return tail_node;
    }
	iterator begin() const{
        iterator ret(head);
        ++ret;
        return (ret);
	}
	iterator end() const{
        iterator ret(tail);
        return ret;
	}
    iterator insert(iterator &pos, const T &val){//insert a node before this node, return an iterator pointing to the newly inserted node
        Node<T> *now_ele = pos.ptr;
        Node<T> *pre_ele = now_ele->pre;
        Node<T> *new_node = new Node<T>(new T(val), pre_ele, now_ele);
        pre_ele->nxt = new_node;
        now_ele->pre = new_node;
        pos.ptr = new_node;
        return pos;
    }
	iterator erase(iterator &pos){
        if(pos == head or pos == tail) throw "invalid iterator erase";
        Node<T> *now_ele = pos.ptr;
        Node<T> *pre_ele = now_ele->pre, *nxt_ele = now_ele->nxt;
        now_ele->nxt = now_ele->pre = nullptr;
        delete *(now_ele->info);
        delete now_ele->info;
        delete now_ele;
        pre_ele->nxt = nxt_ele, nxt_ele->pre = pre_ele;
        pos.ptr = nxt_ele;
        return pos;
	}

	/**
	 * the following are operations of double list
	*/
	void insert_head(const T &val){
        Node<T>* new_node = new Node<T>(new T(val), head_node, head_node->nxt);
        head_node->nxt->pre = new_node;
        head_node->nxt = new_node;
	}
	void insert_tail(const T &val){
        Node<T>* new_node = new Node<T>(new T(val), tail_node->pre, tail_node);
        tail_node->pre->nxt = new_node;
        tail_node->pre = new_node;
	}
	void delete_head(){
        iterator it = begin();
        erase(it);
	}
	void delete_tail(){
        iterator it = end();
        erase(--it);
	}
	bool empty(){
        return begin() == end();
	}
    iterator front(){
        return begin();
    }
    iterator back(){
        iterator bk = end();
        return (--bk);
    }

};

    template <class T> class CycleArray {
    public:
        size_t size;
        size_t fore, back;  //[fore, back)
        size_t cap;
        T** arr = nullptr;
        CycleArray(size_t cap) : cap(cap) {
            size = 0;
            fore = back = 0;
            arr = new T*[cap];
        }
        ~CycleArray() {
            for(int i=fore;i!=back;i=(i+1)%cap){
                delete arr[i];
            }
            delete[] arr;
            arr = nullptr;
        }
        void push_back(const T& value){
            ++size;
            arr[back++] = new T(value);
            if(back == cap) back = 0;
        }
        void push_back(T* value){
            ++size;
            // arr[back++] = new T(std::move(*value));
            arr[back++] = value;
            if(back == cap) back = 0;
        }
        T* pop_back(){
            --size;
            if(back == 0) back = cap;
            return arr[--back];
        }
        void push_fore(const T& value){
            ++size;
            if(fore == 0) fore = cap;
            arr[--fore] = new T(value);
        }
        void push_fore(T* value){
            ++size;
            if(fore == 0) fore = cap;
            // arr[--fore] = new T(std::move(*value));
            arr[--fore] = value;
        }
        T* pop_fore(){
            --size;
            T* ret = arr[fore];
            ++fore;
            if(fore == cap) fore = 0;
            return ret;
        }
        T* operator[](size_t pos) const { return arr[pos]; }
        size_t get_dist(size_t from, size_t to) const { return (to - from + cap) % cap; }
        size_t get_size() const { return size; }
        bool is_full() const {return size >= cap - 2;}
    };
    template <class T> class deque {
        using Mylist = double_list<CycleArray<T>*>;

    private:
        int cap = 320;
        int nowsize = 0;

        /* An utility function: split()
         *split oversized block into two smaller blocks, and return an iterator pointing to the first block
         */
        typename Mylist::iterator split(typename Mylist::iterator to_split){
            CycleArray<T> *left = new CycleArray<T>(cap), *right = new CycleArray<T>(cap);
            int left_size = 0;
            iterator it(table, to_split, to_split.getvalue()->arr[to_split.getvalue()->fore], to_split.getvalue()->fore);
            while(left_size <= to_split.getvalue()->size / 2){
                left->push_back((*(it.ptr)));
                ++left_size;
                ++it;
            }
            while(left_size < to_split.getvalue()->size){
                right->push_back((*(it.ptr)));
                ++left_size;
                ++it;
            }
            // while(to_split.getvalue()->get_size()) to_split.getvalue()->pop_back();
            auto ins = table->erase(to_split);
            ins = table->insert(ins, right);
            ins = table->insert(ins, left);
            return ins;
        }
    public:

        Mylist* table;
        class const_iterator;
        class iterator {
        public:
         /* iterator member*/
            Mylist* belong;
            typename Mylist::iterator cont;
            T* ptr;
            int place;

            /**
             * return a new iterator which points to the n-next element.
             * if there are not enough elements, the behaviour is undefined.
             * same for operator-.
             */
            iterator() {
                // belong = nullptr;
            }
            iterator(Mylist* belong, typename Mylist::iterator cont, T* ptr, int place) : belong(belong), cont(cont), ptr(ptr), place(place) {}
            iterator operator+(const int& n) const {
                if(n < 0){
                    return (*this) - (-n);
                }
                iterator ret = (*this);
                int rem = n;
                // std::cout<<"place="<<ret.place<<std::endl;
                // std::cout<<"back="<<ret.cont.getvalue()->back<<std::endl;
                while(rem){
                    if(ret.cont.getvalue()->get_dist(ret.place, ret.cont.getvalue()->back) > rem){
                        // std::cout<<"add with ease!"<<std::endl;
                        ret.place += rem;
                        break;
                    }
                    // std::cout<<"move an element!"<<std::endl;
                    rem -= ret.cont.getvalue()->get_dist(ret.place, ret.cont.getvalue()->back);
                    ++(ret.cont);
                    if((ret.cont).is_end()){
                        --(ret.cont);
                        ret.place = ret.cont.getvalue()->back;
                        if(rem > 0) throw "out of range!";
                    }
                    else{
                        ret.place = ret.cont.getvalue()->fore;
                    }
                }
                ret.ptr = (ret.cont.getvalue()->arr)[ret.place];
                return ret;
            }
            iterator operator-(const int& n) const {
                if(n < 0){
                    return (*this) + (-n);
                }
                iterator ret = (*this);
                int rem = n;
                while(rem){
                    if(ret.cont.getvalue()->get_dist(ret.cont.getvalue()->fore, ret.place) >= rem){
                        ret.place -= rem;
                        break;
                    }
                    rem -= (ret.cont.getvalue()->get_dist(ret.cont.getvalue()->fore, ret.place) + 1);
                    --(ret.cont);
                    ret.place = (ret.cont.getvalue()->back + ret.cont.getvalue()->cap - 1) % ret.cont.getvalue()->cap;
                }
                ret.ptr = (ret.cont.getvalue()->arr)[ret.place];
                return ret;
            }

            /**
             * return the distance between two iterators.
             * if they point to different vectors, throw
             * invaild_iterator.
             */
            int operator-(const iterator& rhs) const {
                if(belong != rhs.belong) throw "invalid_iterator";
                iterator temp = (*this);
                int ret1 = 0;
                while(true){
                    // std::cout<<"getting -"<<std::endl;
                    ret1 += temp.cont.getvalue()->get_dist(temp.place, temp.cont.getvalue()->back);
                    ++(temp.cont);
                    if(temp.cont.is_end()) break;
                    temp.place = temp.cont.getvalue()->fore;
                }

                temp = rhs;
                int ret2 = 0;
                while(true){
                    // std::cout<<"getting -"<<std::endl;
                    ret2 += temp.cont.getvalue()->get_dist(temp.place, temp.cont.getvalue()->back);
                    ++(temp.cont);
                    if(temp.cont.is_end()) break;
                    temp.place = temp.cont.getvalue()->fore;
                }

                return ret2 - ret1;
            }
            iterator& operator+=(const int& n) {
                (*this) = (*this) + n;
                return (*this);
            }
            iterator& operator-=(const int& n) {
                (*this) = (*this) - n;
                return (*this);
            }

            /**
             * iter++
             */
            iterator operator++(int) {
                iterator ret = (*this);
                (*this) = (*this) + 1;
                return ret;
            }
            /**
             * ++iter
             */
            iterator& operator++() {
                (*this) = (*this) + 1;
                return (*this);
            }
            /**
             * iter--
             */
            iterator operator--(int) {
                iterator ret = (*this);
                (*this) = (*this) - 1;
                return ret;
            }
            /**
             * --iter
             */
            iterator& operator--() {
                (*this) = (*this) - 1;
                return (*this);
            }

            /**
             * *it
             */
            T& operator*() const {
                if(ptr == nullptr) throw "bad *";
                return (*ptr);
            }
            /**
             * it->field
             */
            T* operator->() const noexcept {
                return ptr;
            }

            /**
             * check whether two iterators are the same (pointing to the same
             * memory).
             */
            bool operator==(const iterator& rhs) const { return place == rhs.place and cont == rhs.cont; }
            bool operator==(const const_iterator& rhs) const { return place == rhs.place and cont == rhs.cont; }
            /**
             * some other operator for iterators.
             */
            bool operator!=(const iterator& rhs) const { return !((*this) == rhs); }
            bool operator!=(const const_iterator& rhs) const { return !((*this) == rhs); }
        };

        class const_iterator {
            /**
             * it should has similar member method as iterator.
             * you can copy them, but with care!
             * and it should be able to be constructed from an iterator.
             */
        public:
         /* iterator member*/
            Mylist* belong;
            typename Mylist::iterator cont;
            T* ptr;
            int place;

            /**
             * return a new iterator which points to the n-next element.
             * if there are not enough elements, the behaviour is undefined.
             * same for operator-.
             */
            const_iterator() {
                // belong = nullptr;
            }
            const_iterator(const iterator &other) : belong(other.belong), cont(other.cont), ptr(other.ptr), place(other.place) {}
            const_iterator(const const_iterator &other) : belong(other.belong), cont(other.cont), ptr(other.ptr), place(other.place) {}

            const_iterator(Mylist* belong, typename Mylist::iterator cont, T* ptr, int place) : belong(belong), cont(cont), ptr(ptr), place(place) {}
            const_iterator operator+(const int& n) const {
                if(n < 0){
                    return (*this) - (-n);
                }
                const_iterator ret = (*this);
                int rem = n;
                while(rem){
                    if(ret.cont.getvalue()->get_dist(ret.place, ret.cont.getvalue()->back) > rem){
                        ret.place += rem;
                        break;
                    }
                    rem -= ret.cont.getvalue()->get_dist(ret.place, ret.cont.getvalue()->back);
                    ++(ret.cont);
                    if((ret.cont).is_end()){
                        --(ret.cont);
                        ret.place = ret.cont.getvalue()->back;
                    }
                    else{
                        ret.place = ret.cont.getvalue()->fore;
                    }
                }
                ret.ptr = (ret.cont.getvalue()->arr)[ret.place];
                return ret;
            }
            const_iterator operator-(const int& n) const {
                if(n < 0){
                    return (*this) + (-n);
                }
                const_iterator ret = (*this);
                int rem = n;
                while(rem){
                    if(ret.cont.getvalue()->get_dist(ret.cont.getvalue()->fore, ret.place) >= rem){
                        ret.place -= rem;
                        break;
                    }
                    rem -= (ret.cont.getvalue()->get_dist(ret.cont.getvalue()->fore, ret.place) + 1);
                    --(ret.cont);
                    ret.place = (ret.cont.getvalue()->back + ret.cont.getvalue()->cap - 1) % ret.cont.getvalue()->cap;
                }
                ret.ptr = (ret.cont.getvalue()->arr)[ret.place];
                return ret;
            }

            /**
             * return the distance between two iterators.
             * if they point to different vectors, throw
             * invaild_iterator.
             */
            int operator-(const const_iterator& rhs) const {
                if(belong != rhs.belong) throw "invalid_iterator";
                const_iterator temp = (*this);
                int ret1 = 0;
                while(true){
                    ret1 += temp.cont.getvalue()->get_dist(temp.place, temp.cont.getvalue()->back);
                    ++(temp.cont);
                    if(temp.cont.is_end()) break;
                    temp.place = temp.cont.getvalue()->fore;
                }

                temp = rhs;
                int ret2 = 0;
                while(true){
                    ret2 += temp.cont.getvalue()->get_dist(temp.place, temp.cont.getvalue()->back);
                    ++(temp.cont);
                    if(temp.cont.is_end()) break;
                    temp.place = temp.cont.getvalue()->fore;
                }

                return ret2 - ret1;
            }
            const_iterator& operator+=(const int& n) {
                (*this) = (*this) + n;
                return (*this);
            }
            const_iterator& operator-=(const int& n) {
                (*this) = (*this) - n;
                return (*this);
            }

            /**
             * iter++
             */
            const_iterator operator++(int) {
                const_iterator ret = (*this);
                (*this) = (*this) + 1;
                return ret;
            }
            /**
             * ++iter
             */
            const_iterator& operator++() {
                (*this) = (*this) + 1;
                return (*this);
            }
            /**
             * iter--
             */
            const_iterator operator--(int) {
                const_iterator ret = (*this);
                (*this) = (*this) - 1;
                return ret;
            }
            /**
             * --iter
             */
            const_iterator& operator--() {
                (*this) = (*this) - 1;
                return (*this);
            }

            /**
             * *it
             */
            T& operator*() const {
                return (*ptr);
            }
            /**
             * it->field
             */
            T* operator->() const noexcept {
                return ptr;
            }

            /**
             * check whether two iterators are the same (pointing to the same
             * memory).
             */
            bool operator==(const iterator& rhs) const { return place == rhs.place and cont == rhs.cont; }
            bool operator==(const const_iterator& rhs) const { return place == rhs.place and cont == rhs.cont; }
            /**
             * some other operator for iterators.
             */
            bool operator!=(const iterator& rhs) const { return !((*this) == rhs); }
            bool operator!=(const const_iterator& rhs) const { return !((*this) == rhs); }
        };
        /*
            utilities.
        */
        // void travel(){
        //     std::cout<<"-----------travel------------"<<std::endl;
        //     int block_cnt = 0;
        //     for(auto it=table->begin();it!=table->end();++it){
        //         std::cout<<"block_id="<<++block_cnt<<" [";
        //         for(int i=it.getvalue()->fore;i!=it.getvalue()->back;(i=(i+1)%it.getvalue()->cap)) {
        //             // (*(it.getvalue()->arr[i])).print();
        //             std::cout<<(*(it.getvalue()->arr[i]));
        //             // (*(it.getvalue()->arr[i])).print();
        //             std::cout<<',';
        //         }
        //         std::cout<<"]"<<std::endl;
        //     }
        // }
        /**
         * constructors.
         */
        deque() {
            table = new Mylist;
            CycleArray<T>* firstArray = new CycleArray<T>(cap);
            table->insert_tail(firstArray);
        }
        deque(const deque& other) {
            table = new Mylist;
            cap = other.cap;
            CycleArray<T>* firstArray = new CycleArray<T>(cap);
            table->insert_tail(firstArray);
            for(auto it=other.begin();it!=other.end();++it){
                push_back((*it));
            }
        }

        /**
         * deconstructor.
         */
        ~deque() {
            // while(nowsize){
            //     pop_back();
            // }
            // auto it = table->head;
            // table->erase(++it);
            delete table;
            table = nullptr;
        }

        /**
         * assignment operator.
         */
        deque& operator=(const deque& other) {
            if(&other == this) return (*this);
            delete table;
            nowsize = 0;
            table = new Mylist;
            cap = other.cap;
            CycleArray<T>* firstArray = new CycleArray<T>(cap);
            table->insert_tail(firstArray);
            for(auto it=other.begin();it!=other.end();++it){
                push_back((*it));
                // travel();
            }
            return (*this);
        }

        /**
         * access a specified element with bound checking.
         * throw index_out_of_bound if out of bound.
         */
        T& at(const size_t& pos) {
            if(pos >= nowsize) throw "index_out_of_bound";
            return (*(begin() + pos));
        }
        const T& at(const size_t& pos) const {
            if(pos >= nowsize) throw "index_out_of_bound";
            return (*(cbegin() + pos));
        }
        T& operator[](const size_t& pos) {
            if(pos >= nowsize) throw "index_out_of_bound";
            return (*(begin() + pos));
        }
        const T& operator[](const size_t& pos) const {
            if(pos >= nowsize) throw "index_out_of_bound";
            return (*(cbegin() + pos));
        }

        /**
         * access the first element.
         * throw container_is_empty when the container is empty.
         */
        const T& front() const {
            if(nowsize == 0) throw "container_is_empty";
            return (*begin());
        }
        /**
         * access the last element.
         * throw container_is_empty when the container is empty.
         */
        const T& back() const {
            if(nowsize == 0) throw "container_is_empty";
            return (*(end() - 1));
        }

        /**
         * return an iterator to the beginning.
         */
        iterator begin() const { return iterator(table, table->front(), table->front().getvalue()->arr[table->front().getvalue()->fore], table->front().getvalue()->fore); }
        const_iterator cbegin() const { return const_iterator(begin()); }

        /**
         * return an iterator to the end.
         */
        iterator end() const {
            // return iterator(table, table->back(), nullptr, (table->back().getvalue()->back + table->back().getvalue()->cap + 1) % (table->back().getvalue()->cap));
            return iterator(table, table->back(), nullptr, table->back().getvalue()->back);
        }
        const_iterator cend() const {
            return const_iterator(end());
        }

        /**
         * check whether the container is empty.
         */
        bool empty() const { return nowsize == 0; }

        /**
         * return the number of elements.
         */
        size_t size() const { return nowsize; }

        /**
         * clear all contents.
         */
        void clear() {
            while(size()){
                pop_back();
            }
        }

        /**
         * insert value before pos.
         * return an iterator pointing to the inserted value.
         * throw if the iterator is invalid or it points to a wrong place.
         */
        iterator insert(iterator pos, const T& value) {
            if(pos.belong != table) throw "invalid iterator";
            // T** save = new T*[pos.cont.getvalue()->cap + 5];
            int save_tot = pos.cont.getvalue()->get_dist(pos.place, pos.cont.getvalue()->back);
            // std::cout<<"save_tot="<<save_tot<<std::endl;
            ++(pos.cont.getvalue()->size);
            ++(pos.cont.getvalue()->back);
            if(pos.cont.getvalue()->back == pos.cont.getvalue()->cap) 
                pos.cont.getvalue()->back = 0;
            for(int i=(pos.place+save_tot-1)%pos.cont.getvalue()->cap, nowins=0;nowins!=save_tot;i=(i-1)%pos.cont.getvalue()->cap,++nowins){
                // std::cout<<"i="<<i<<std::endl;
                pos.cont.getvalue()->arr[(i+1)%pos.cont.getvalue()->cap] = pos.cont.getvalue()->arr[i];
            }
            pos.cont.getvalue()->arr[pos.place] = new T(value);
            ++nowsize;
            if(pos.cont.getvalue()->is_full()){
                int rela_place = pos.cont.getvalue()->get_dist(pos.cont.getvalue()->fore, pos.place);
                pos.cont = split(pos.cont);
                pos.place = rela_place;
                if(rela_place >= pos.cont.getvalue()->size){
                    pos.place = rela_place - pos.cont.getvalue()->size;
                    ++pos.cont;
                }
            }
            // delete[] save;
            pos.ptr = pos.cont.getvalue()->arr[pos.place];
            return pos;
        }

        /**
         * remove the element at pos.
         * return an iterator pointing to the following element. if pos points to
         * the last element, return end(). throw if the container is empty,
         * the iterator is invalid, or it points to a wrong place.
         */
        iterator erase(iterator pos) {
            if(pos.belong != table){
                // std::cout<<"wrong container"<<std::endl;
                throw "wrong container";
            }
            if(pos == end()){
                throw "bad erase at end()";
            }
            if(pos.cont.getvalue()->size == 0){
                // std::cout<<"pointing to empty container"<<std::endl;
                throw "pointing to empty container";
            }

            // T** save = new T*[pos.cont.getvalue()->cap + 5];
            T* save[pos.cont.getvalue()->size + 5];
            int save_tot = pos.cont.getvalue()->get_dist(pos.place, pos.cont.getvalue()->back) - 1;
            for(int i=1;i<=save_tot;i++){
                save[i] = pos.cont.getvalue()->pop_back();
            }
            auto erased = pos.cont.getvalue()->pop_back();
            delete erased;
            --nowsize;
            for(int i=save_tot;i>=1;i--){
                pos.cont.getvalue()->push_back(save[i]);
            }
            pos.ptr = pos.cont.getvalue()->arr[pos.place];
            if(nowsize == 0){
            // std::cout<<"mid_check"<<std::endl;
                // delete[] save;
                return end();
            }
            if(pos.cont.getvalue()->size == 0){
                auto to_delete = pos.cont;
                pos.cont = table->erase(to_delete);
                if(pos.cont == table->end()){
                    pos = end();
                    // delete[] save;
                    return pos;
                }
                pos.place = pos.cont.getvalue()->fore;
                    // delete to_delete;
            }
            if(pos.place == pos.cont.getvalue()->back){
                ++pos.cont;
                if(pos.cont == table->end()){
                    pos = end();
                    // delete[] save;
                    return pos;
                }
                pos.place = pos.cont.getvalue()->fore;
            }
            pos.ptr = pos.cont.getvalue()->arr[pos.place];
            // delete[] save;
            return pos;
        }

        /**
         * add an element to the end.
         */
        void push_back(const T& value) {
            insert(end(), value);
        }

        /**
         * remove the last element.
         * throw when the container is empty.
         */
        void pop_back() {
            // std::cout<<"nowsize="<<nowsize<<std::endl;
            // std::cout<<"pop_back()"<<std::endl;
            // travel();
            if(nowsize == 0) throw "container empty";
            erase(end() - 1);
        }

        /**
         * insert an element to the beginning.
         */
        void push_front(const T& value) {
            insert(begin(), value);
        }

        /**
         * remove the first element.
         * throw when the container is empty.
         */
        void pop_front() {
            // std::cout<<"pop_front()"<<std::endl;
            // travel();
            if(nowsize == 0) throw "container empty";
            erase(begin());
        }
    };

} // namespace sjtu

#endif
