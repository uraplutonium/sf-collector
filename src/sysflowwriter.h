#ifndef __SF_WRITER_
#define __SF_WRITER_

#include "avro/ValidSchema.hh"
#include "avro/Compiler.hh"
#include "avro/DataFile.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

#include "sysflow/sysflow.hh"
#include "sysflowcontext.h"
#include "utils.h"
#define COMPRESS_BLOCK_SIZE 80000
using namespace sysflow;

class SysFlowWriter {
    private:
        SysFlowContext* m_cxt;
        SysFlow m_flow;
        int m_numRecs;
        avro::ValidSchema m_sysfSchema;
        avro::DataFileWriter<SysFlow>* m_dfw; 
        void writeHeader();
        time_t m_start;
        string getFileName(time_t curTime);
    public:
        SysFlowWriter(SysFlowContext* cxt, time_t start);
        virtual ~SysFlowWriter();
        inline int getNumRecs() {
            return m_numRecs;
        }
        inline void writeContainer(Container* container) {
             m_flow.rec.set_Container(*container);
             m_numRecs++;
             m_dfw->write(m_flow);
        }
        inline void writeProcess(Process* proc) {
            cout << "Write Proc: " << proc->oid.hpid << " " << proc->oid.createTS << " "  << proc->exe << " " << proc->exeArgs << endl;
            m_flow.rec.set_Process(*proc);
            m_numRecs++;
            m_dfw->write(m_flow);
        }
        inline void writeProcessFlow(ProcessFlow* pf) {
            m_flow.rec.set_ProcessFlow(*pf);
            m_numRecs++;
            m_dfw->write(m_flow);  
        }
        inline void writeNetFlow(NetworkFlow* nf) {
            m_flow.rec.set_NetworkFlow(*nf);
            m_numRecs++;
            m_dfw->write(m_flow);
        }
        inline void writeFileFlow(FileFlow* ff) {
            m_flow.rec.set_FileFlow(*ff);
            m_numRecs++;
            m_dfw->write(m_flow);
        }
        inline void writeAtomicFileFlow(AtomicFileFlow* ff) {
            m_flow.rec.set_AtomicFileFlow(*ff);
            m_numRecs++;
            m_dfw->write(m_flow);
        }
        inline void writeFile(File* f) {
            m_flow.rec.set_File(*f);
            m_numRecs++;
            m_dfw->write(m_flow);
        }
        inline bool isFileExpired(time_t curTime) {
             if(m_start > 0) {
                double duration = getDuration(curTime);
                return (duration >= m_cxt->getFileDuration());
             }
             return false;
         }
        inline double getDuration(time_t curTime) {
            return difftime(curTime, m_start);
        }
        int initialize();
        void resetFileWriter(time_t curTime);
};

#endif